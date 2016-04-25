#include "Terrain.h"
#include "Misc.h"

Terrain::Terrain() {
  
}


Terrain::Terrain(const Terrain& orig) {
}


Terrain::~Terrain() {
}


void Terrain::draw() {
  //draw boxes
  ESAT::DrawSetStrokeColor(255,255,255,255);
  
  float pathpoints[5000];
  
  
  
  int ta, tb;
  
  
  int i,k = 0;
  int inner_index;
  for (i=position_index_; i<num_onscreen_points_ + position_index_; i++) {
    
    inner_index = i % num_terrain_points_;
    
    
    //Calculate upper-mid point
    int x = points_[inner_index].x - points_[position_index_].x;
    if (i >= num_terrain_points_) {
      x = k * onscreen_point_width_;
    }
    
    ta = x;
    
    pathpoints[(2*k)+0] = x;
    pathpoints[(2*k)+1] = points_[inner_index].y;
    
    //Draw sign if this is a landing spot
    for (int j=0; j<landings_.size(); j++) {
      if (inner_index == landings_[j].begin) {
        //int x = onscreen_point_width_ * (inner_index + (landings_[j].size/2));
        int y = points_[inner_index].y;
        
        std::string points = std::to_string(landings_[j].points);
        ESAT::DrawText(x, y, points.c_str());
      }
    }
    
    k++;
    
  }
//ESAT::DrawText(300, 300, std::to_string(inner_index).c_str());
//ESAT::DrawText(300, 400, std::to_string(ta).c_str());
    
  //draw pathpoints
  ESAT::DrawPath(pathpoints, num_onscreen_points_);
}


void Terrain::scroll(float amount) {
  position_screen_ += 0;
  
  
  ESAT::DrawText(500.0f, 550.0f, ("Terrain X: "+std::to_string(position_screen_+683.0f)).c_str());
  position_index_ = (int)(position_screen_ / onscreen_point_width_);
  
  
  if (position_index_ > num_terrain_points_)
    position_index_ = position_index_ % num_terrain_points_;
  else if (position_index_ < 0)
    position_index_ = (num_terrain_points_ + position_index_)%num_terrain_points_;
  
  //Correct negative screen positions
  if (position_index_ < 0)
    position_index_ = num_terrain_points_ + position_index_;
  
  ESAT::DrawText(500.0f, 600.0f, ("Pos index: "+std::to_string(position_index_)).c_str());
}

void Terrain::createLandingSpot(int begin, int size) {
  LandingSpot land;
  
  land.begin = begin;
  land.size = size;
  land.end = land.begin + land.size;
  land.points = 15 * (land.size/10);
  
  landings_.insert (landings_.begin(), land);
}


void Terrain::generate() {
  
  onscreen_point_width_ = (float)kWinWidth / (float)num_onscreen_points_;
  
  int spots[] = {100, 250, 400};
  
  for (int i=0; i < 3; i++) {
    
    int a = spots[i];
    int begin = Misc::random(a-5, a+5);
    int size = Misc::random(num_onscreen_points_/30, num_onscreen_points_/20);
  
    createLandingSpot(begin, size);
  }
  
  
  
  float randomness = Misc::random(0,100);
  
  for (int i=0; i<num_terrain_points_; i++) {

    /*********************/
    float pos_x = onscreen_point_width_ * i;
    float height = stb_perlin_noise3((i+1)/num_terrain_points_, (float)pos_x/100, randomness, 0, 0, 0);
    /*********************/
            
    bool landing_zone = false;
    
    for (int j=0; j < landings_.size() && !landing_zone; j++) {
      if (i >= landings_[j].begin && i <= landings_[j].end) {
        landing_zone = true;
      }
    }
    if (landing_zone) {
      //Duplicate last height
      height = points_[points_.size()-1].y;
    } else {
      height = (height * 100) + 650;
    }
    
    MathLib::Point2 p = {pos_x, height};
    points_.insert (points_.end(), p);
  }
}
  
  
void Terrain::createWalls() {
  
  for(int i=0; i<points_.size()-1; i+=2) {
    
    MathLib::Point2 p1 = points_[i];
    MathLib::Point2 p2 = points_[i+1];
  
    cpShape* ground = cpSegmentShapeNew( cpSpaceGetStaticBody(space_), cpv(p1.x, p1.y), cpv(p2.x, p2.y), 0.0f);
    
    bool landing_zone = false;
    
    for (int j=0; j < landings_.size() && !landing_zone; j++) {
      if (i >= landings_[j].begin && i <= landings_[j].end) {
        landing_zone = true;
        landings_[j].shapes.insert(landings_[j].shapes.end(), ground);
      }
    }
    
    if (landing_zone) {
      cpShapeSetCollisionType(ground, LANDING_TYPE);
    } else {
      cpShapeSetCollisionType(ground, GROUND_TYPE);
    }
    
    cpShapeSetFriction(ground, 1.0f);
    cpSpaceAddShape(space_, ground);
  }
}