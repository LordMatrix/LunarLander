/* Copyright 2016 Marcos Vazquez. All rights reserved.
 *
 * Author: Marcos Vazquez <vazquezre@esat-alumni.es>
 *
 * File:   main.cc               
 * Lunar Lander fork with chipmunk physics
 *
 */

//#ifndef WIN32
//#define WIN32 1
//#endif

#include <MOMOS/window.h>
#include <MOMOS/draw.h>
#include <MOMOS/input.h>
 
#include <iostream>
#include "time.h"
#include "../include/Ship.h"

#define STB_PERLIN_IMPLEMENTATION
#include "../include/Terrain.h"
 
#include <chipmunk/chipmunk.h>
 
#include "../include/config.h"
#include "../include/LRV.h"

MOMOS::SpriteHandle bg;

struct {
  int width;
  int height;
} WindowOptions = {kWinWidth, kWinHeight};

typedef struct {
    Ship* ship;
    Terrain* terrain;
  } Colliders;
  
int g_score = 0;
float g_time = 0.0f;
bool g_just_landed;

cpSpace* g_space = nullptr;
Ship* g_ship = nullptr;
LRV* g_lrv = nullptr;
Vehicle* g_vehicle = nullptr;
Terrain* g_terrain = nullptr;
Colliders* g_colliders = nullptr;


/**
 * @brief Draws numerical info about the game status
 * @param ship  The current Ship to be examined
 */
void drawInfo(Ship* ship) {

  std::string score_str ("SCORE  "+std::to_string(g_score));
  std::string time_str  ("TIME   "+std::to_string(g_time));
  std::string fuel_str  ("FUEL   "+std::to_string(ship->fuel_));
  std::string alt_str   ("ALTITUDE            "+std::to_string(kWinHeight - ship->pos_.y).substr(0,5));
  std::string hspeed_str("HORIZONTAL SPEED    "+std::to_string(fabs(ship->velocity_.x)).substr(0,5));
  std::string vspeed_str("VERTICAL SPEED      "+std::to_string(fabs(ship->velocity_.y)).substr(0,5));
  
  hspeed_str += (ship->velocity_.x > 0.0f) ? "   >" : "   <";
  vspeed_str += (ship->velocity_.y > 0.0f) ? "   -" : "   +";
  
  MOMOS::DrawText(100.0f, 50.0f, score_str.c_str());
  MOMOS::DrawText(100.0f, 75.0f, time_str.c_str());
  MOMOS::DrawText(100.0f, 100.0f, fuel_str.c_str());
  
  MOMOS::DrawText(800.0f, 50.0f, alt_str.c_str());
  MOMOS::DrawText(800.0f, 75.0f, hspeed_str.c_str());
  MOMOS::DrawText(800.0f, 100.0f, vspeed_str.c_str());
}


/**
 * Gets called every time the Ship collides with non-landing terrain.
  * Unspecified parameters are filled automatically by chipmunk.
 * @param arb
 * @param space   The physics simulation cpSpace
 * @param data
 * @return 
 */
cpBool OnCrashingCollisionEnter(cpArbiter *arb, cpSpace *space, void *data) {
  g_ship->crashed_ = true;
  g_ship->landed_ = true;
  //If false, collision is interrupted
  return cpTrue;
}  


/**
 * @brief This gets called every time the Ship collides with landing terrain.
 * It updates the g_ship status.
 * Unspecified parameters are filled automatically by chipmunk.
 * @param arb
 * @param space   The physics simulation cpSpace
 * @param data
 * @return 
 */
cpBool OnLandingCollisionEnter(cpArbiter *arb, cpSpace *space, void *data) {
  cpShape *a = NULL;
  cpShape *b = NULL;
  cpBody *c = NULL;
  cpBody *d = NULL;
  
  cpArbiterGetShapes(arb, &a, &b);
  cpArbiterGetBodies(arb, &c, &d);

  Colliders* collider = reinterpret_cast<Colliders*>(data);
  
  Ship* ship = collider->ship;
  Terrain* terrain = collider->terrain;
  
  //Order of colliders is undeterministic
  if (!ship->landed_ && !ship->crashed_) {
    cpBody* s = ship->physics_body_;
    cpVect velocity = cpBodyGetVelocity(s);
    cpVect rotation = cpBodyGetRotation(s);
    ship->landed_ = true;
    
    float velocity_threshold = 0.9f;
    float rotation_threshold = 0.9f;
    
    bool speed_ok = fabs(velocity.x)<velocity_threshold && fabs(velocity.y)<velocity_threshold;
    bool rotation_ok = fabs(1 - rotation.x)<rotation_threshold && fabs(rotation.y)<rotation_threshold;
    
    //DEBUG MESSAGES
    if (speed_ok) printf("Speed OK\n");
    else printf("Ship is going too fast\n");
    
    if (rotation_ok) printf("Rotation OK\n");
    else printf("Ship is upside down\n");
    
    if (speed_ok && rotation_ok) {

      Terrain::LandingSpot* landing = nullptr;

      //Check if terrain's box is a LandingSpot
      for (int i=0; i < terrain->landings_.size() && landing==nullptr; i++) {

        for (int j=0; j < terrain->landings_[i].shapes.size() && landing==nullptr; j++) {

          if (terrain->landings_[i].shapes[j] == a || terrain->landings_[i].shapes[j] == b) {
            landing = &terrain->landings_[i];
            g_score += landing->points;

            //Mark ship to be removed
            g_just_landed = true;
          } 
        } 
      }
    } else {
      //The ship is either going too fast or upside down
      ship->crashed_ = true;
    }
  }
  
  //If false, collision is interrupted
  return cpTrue;
}


/**
 * Gets called every time the LRV collides with any terrain.
 * Locks the LRV controls, preventing player interaction-
 * Unspecified parameters are filled automatically by chipmunk.
 * @param arb
 * @param space   The physics simulation cpSpace
 * @param data
 * @return 
 */
cpBool OnLRVCollisionEnter(cpArbiter *arb, cpSpace *space, void *data) {
  g_lrv->locked_ = false;
  return cpTrue;
}


/**
 * Gets called every time the LRV ends collision (separates) from any terrain.
 * Locks the LRV controls, preventing player interaction-
 * Unspecified parameters are filled automatically by chipmunk.
 * @param arb
 * @param space   The physics simulation cpSpace
 * @param data
 * @return 
 */
void OnLRVCollisionExit(cpArbiter *arb, cpSpace *space, void *data) {
  g_lrv->locked_ = true;
}


///Creates collision handlers for ship, lvr & terrain.
void createCollisionHandlers() {
  //Create landing collision
  cpCollisionHandler* handler = cpSpaceAddCollisionHandler (g_space, SHIP_TYPE, LANDING_TYPE);
  handler->beginFunc = OnLandingCollisionEnter;
  handler->userData = g_colliders;

  //Create crashing collision
  handler = cpSpaceAddCollisionHandler (g_space, SHIP_TYPE, GROUND_TYPE);
  handler->beginFunc = OnCrashingCollisionEnter;
  handler->userData = g_colliders;

  //Create LRV collisions
  handler = cpSpaceAddCollisionHandler (g_space, LRV_TYPE, GROUND_TYPE);
  handler->preSolveFunc = OnLRVCollisionEnter;
  handler->userData = g_colliders;

  handler = cpSpaceAddCollisionHandler (g_space, LRV_TYPE, GROUND_TYPE);
  handler->separateFunc = OnLRVCollisionExit;
  handler->userData = g_colliders;

  handler = cpSpaceAddCollisionHandler (g_space, LRV_TYPE, LANDING_TYPE);
  handler->preSolveFunc = OnLRVCollisionEnter;
  handler->userData = g_colliders;

  handler = cpSpaceAddCollisionHandler (g_space, LRV_TYPE, LANDING_TYPE);
  handler->separateFunc = OnLRVCollisionExit;
  handler->userData = g_colliders;
}


/**
 * Initializes variables and sets the game to its initial status
 */
void startGame() {
  srand(time(NULL));
  
  g_just_landed = false;
  
  g_space = cpSpaceNew();
  g_ship = new Ship();
  g_lrv = new LRV();
  g_terrain = new Terrain();
  g_colliders = (Colliders*)malloc(sizeof(Colliders));
  
  cpSpaceSetGravity(g_space, cpv(0, 0.0098f));

  //Create Ship
  g_ship->space_ = g_space;
  MathLib::Vec2 offset = {0.0f, 0.0f};
  g_ship->assignRegularPolygon(20, 10, offset, 0, g_ship->cvertices_, &g_ship->num_cvertices_);
  offset.y = 5.0f;
  g_ship->assignRegularPolygon(3, 10, offset, 225, g_ship->tvertices_, &g_ship->num_tvertices_);
  g_ship->setPhysics();
  
  
  //Create Vehicle
  g_lrv->space_ = g_space;
  g_lrv->assignRectangle(g_lrv->qvertices_, &g_lrv->num_qvertices_);
  g_lrv->setPhysics();
  
  
  //Create Terrain
  g_terrain->space_ = g_space;
  g_terrain->generate();
  g_terrain->createWalls();
  
  g_colliders->ship = g_ship;
  g_colliders->terrain = g_terrain;
  
  
  createCollisionHandlers();
  
  g_time = 0.0f;
}


/**
 * Updates the game status
 * @param delta   Milliseconds elapsed since the last update
 */
void update(double delta) {
  
//    g_lrv->locked_ = true;
    cpSpaceStep(g_space, delta);
    
    g_ship->pos_.x = cpBodyGetPosition(g_ship->physics_body_).x;
    g_ship->pos_.y = cpBodyGetPosition(g_ship->physics_body_).y;
    
    g_lrv->pos_.x = cpBodyGetPosition(g_lrv->physics_body_).x;
    g_lrv->pos_.y = cpBodyGetPosition(g_lrv->physics_body_).y;
    /*****************************************/
    
    //Restart game if...
    if ((g_ship->crashed_ || g_ship->landed_)  && MOMOS::IsSpecialKeyPressed(MOMOS::kSpecialKey_Space)) {
      startGame();
    }
    
    //Update ship according to its status
    if (g_just_landed) {
      g_ship->removePhysics();
      g_just_landed = false;
      g_ship->velocity_.x = 0;

      cpVect position = {g_ship->pos_.x, g_ship->pos_.y};
      cpBodySetPosition(g_lrv->physics_body_, position);
    }
    g_ship->update();
    g_lrv->update();
     
    
    if (!g_ship->landed_ || g_ship->crashed_) {
      g_vehicle = static_cast<Ship*>(g_ship);
    } else {
      g_vehicle = static_cast<LRV*>(g_lrv);
    }
    
    
    //Scroll terrain
    g_terrain->position_screen_ = g_vehicle->pos_.x - 683.0f;
    g_terrain->scroll();

    //Correct the ship's coordinates and make it loop through the terrain
    if (g_vehicle->pos_.x<0) {
      g_vehicle->pos_.x = g_terrain->onscreen_point_width_ * g_terrain->num_terrain_points_;
      cpBodySetPosition(g_vehicle->physics_body_, {g_vehicle->pos_.x, g_vehicle->pos_.y});
    }

    //If ship's X exceeds map
    float limit = g_terrain->onscreen_point_width_ * g_terrain->num_terrain_points_;
    if (g_vehicle->pos_.x > limit) {
      g_vehicle->pos_.x = 0;
      cpBodySetPosition(g_vehicle->physics_body_, {g_vehicle->pos_.x, g_vehicle->pos_.y});
    }
    
    //Correct static position of ship once it's landed
    if (g_ship->landed_ && !g_ship->crashed_) {
      float repos_index = g_terrain->position_index_*g_terrain->onscreen_point_width_;
      g_ship->pos_.x  -= repos_index;
    }
}


/**
 * Draws the game status
 */
void draw() {
  MOMOS::DrawBegin();
  MOMOS::DrawClear(0,0,0);

  MOMOS::DrawSprite(bg, 0, 0);
  drawInfo(g_ship);

  g_terrain->draw();
  g_ship->draw();

  if (g_ship->landed_ && !g_ship->crashed_)
    g_lrv->draw();

  MOMOS::DrawEnd();
  MOMOS::WindowFrame();
}


/**
 * @brief The alpha and the omega 
 */
int main(int argc, char **argv) {
  
  startGame();
  
  MOMOS::WindowInit(WindowOptions.width, WindowOptions.height);
  
  bg = MOMOS::SpriteFromFile("assets/img/sky.jpg");
  
  //Init font
  //MOMOS::DrawSetTextFont("assets/font/medieval.ttf");
  MOMOS::DrawSetTextFont("src/asteroids/assets/ca.ttf");
  MOMOS::DrawSetTextSize(20);
  
  static double last_time = MOMOS::Time();
  
  while(MOMOS::WindowIsOpened() && !MOMOS::IsSpecialKeyDown(MOMOS::kSpecialKey_Escape)) {

    /****************SIMULATION****************/
    double tick = MOMOS::Time();
    double delta = (tick - last_time) * 0.1f;
    last_time = MOMOS::Time();
    
    if (!g_ship->crashed_ && !g_ship->landed_)
      g_time += delta/100;
            
    update(delta*1000);
    draw();
  }
  MOMOS::WindowDestroy();
  return 0;
}