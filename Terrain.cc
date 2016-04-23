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
  
  for (int i=0; i<num_points_; i++) {
    
    //Calculate upper-mid point
    pathpoints[(2*i)+0] = points_[i].x;
    pathpoints[(2*i)+1] = points_[i].y;
    
    //Draw sign if this is a landing spot
    for (int j=0; j<landings_.size(); j++) {
      if (i == landings_[j].begin) {
        int x = point_width_ * (i + (landings_[j].size/2));
        int y = points_[i].y;
        
        std::string points = std::to_string(landings_[j].points);
        ESAT::DrawText(x, y, points.c_str());
      }
    }
  }
  
  //draw pathpoints
  ESAT::DrawPath(pathpoints, num_points_);
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
  
  point_width_ = (float)kWinWidth / (float)num_points_;
  
  int spots[] = {100, 250, 400};
  
  for (int i=0; i < 3; i++) {
    
    int a = spots[i];
    int begin = Misc::random(a-5, a+5);
    int size = Misc::random(num_points_/30, num_points_/20);
  
    createLandingSpot(begin, size);
  }
  
  
  
  
  
  for (int i=0; i<num_points_; i++) {

    /*********************/
    float pos_x = point_width_ * i;
    float height = stb_perlin_noise3((i+1)/num_points_, (float)pos_x/50, 0.0f, 0, 0, 0);
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