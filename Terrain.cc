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
  
  float pathpoints[500];
  
  for (int i=0; i<num_boxes_; i++) {
    float points[10];
    
    for (int j=0; j<8; j+=2) {
      int a = boxes_[i].vertices[j/2].x;
      points[j+0] = boxes_[i].vertices[j/2].x;
      points[j+1] = boxes_[i].vertices[j/2].y;
    }
    
    points[8] = points[0];
    points[9] = points[1];
    
    //Calculate upper-mid point
    pathpoints[(2*i)+0] = (points[4] + points[6]) / 2;
    pathpoints[(2*i)+1] = points[5];
    
//    ESAT::DrawSolidPath(points, 5, true);
//    ESAT::DrawText(points[6], points[7], std::to_string(i).c_str());
    
    //Draw sign if this is a landing spot
    for (int j=0; j<landings_.size(); j++) {
      if (i == landings_[j].begin) {
        int x = box_width_ * (i + (landings_[j].size/2));
        int y = boxes_[i].vertices[2].y;
        
        std::string points = std::to_string(landings_[j].points);
        ESAT::DrawText(x, y, points.c_str());
      }
    }
  }
  
  //draw pathpoints
  ESAT::DrawPath(pathpoints, num_boxes_);
}


void Terrain::generate() {
  //generate boxes
  int box_min_height = 10;
  int box_max_height = 500;
  int last_height = 100;
  int steepness = 50;
  
  box_width_ = (float)kWinWidth / (float)num_boxes_;
  
  //Define landing spots
  LandingSpot land;
  
  land.begin = Misc::random(5, 10);
  land.size = Misc::random(3, 5);
  land.end = land.begin + land.size;
  land.points = 5 * (10 - land.size);
  landings_.insert (landings_.begin(), land);
  
  land.begin = Misc::random(40, 50);
  land.size = Misc::random(3, 5);
  land.end = land.begin + land.size;
  land.points = 5 * (10 - land.size);
  landings_.insert (landings_.begin(), land);
  
  land.begin = Misc::random(70, 90);
  land.size = Misc::random(5, 7);
  land.end = land.begin + land.size;
  land.points = 5 * (10 - land.size);
  landings_.insert (landings_.begin(), land);
  
  for (int i=0; i<num_boxes_; i++) {
    //Create a single box
    
    bool landing_zone = false;
    
    for (int j=0; j < landings_.size() && !landing_zone; j++) {
      if (i >= landings_[j].begin && i <= landings_[j].end) {
        landing_zone = true;
      }
    }
    steepness = (landing_zone) ? 0 : 50;
    
    int diff = Misc::random(steepness) * Misc::random_sign() +200;
    float box_height = (box_min_height+diff) % box_max_height;
    last_height = box_height;
    
    Box box;
    box.vertices[0] = {box_width_*(i+0),kWinHeight};
    box.vertices[1] = {box_width_*(i+1), kWinHeight};
    box.vertices[2] = {box_width_*(i+1),kWinHeight - box_height};
    box.vertices[3] = {box_width_*(i+0),kWinHeight - box_height};
    boxes_.insert (boxes_.end(), box);
  }
}
  
  
void Terrain::createWalls() {
  
  for(int i=0; i<boxes_.size()-1; i++) {
    Box box1 = boxes_[i];
    Box box2 = boxes_[i+1];
    
    MathLib::Point2 p1 = {box1.vertices[3].x + (box1.vertices[2].x - box1.vertices[3].x)/2, box1.vertices[2].y};
    MathLib::Point2 p2 = {box2.vertices[3].x + (box2.vertices[2].x - box2.vertices[3].x)/2, box2.vertices[2].y};
  
//    MathLib::Point2 p1 = {box1.vertices[3].x, box1.vertices[3].y};
//    MathLib::Point2 p2 = {box2.vertices[3].x, box2.vertices[3].y};
    
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