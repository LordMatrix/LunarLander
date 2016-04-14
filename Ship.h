//#ifndef WIN32
//#define WIN32 1
//#endif

#ifndef __SHIP_H__
#define __SHIP_H__ 1

#include <vector>
#include <cmath>
#include <ctgmath>
#include "MathLib.h"
#include <ESAT/math.h>
#include <ESAT/time.h>
#include "ESAT_extra/chipmunk/chipmunk.h"
#include "config.h"

class Ship {
  public:
    Ship();
    Ship(const Ship& orig);
    virtual ~Ship();
    
    void assignRegularPolygon(int num_vertices, float radius, MathLib::Vec2 position_offset, float rotation_offset, std::vector<MathLib::Vec2>& buffer, int* counter);
    void update();
    void move();
    void drawThruster();
    void draw();
    void setPhysics();
    
    MathLib::Vec2 pos_;
    MathLib::Vec2 velocity_;
    
    double last_update_;
    
    cpSpace* space_;
    
    cpBody* physics_body_;
    
    float fuel_;
    
    bool thrusting_;
    
    bool landed_;
    bool crashed_;
    bool exploding_;
    
    float explosion_time_;
    
    std::vector<MathLib::Vec2> cvertices_;
    int num_cvertices_;
    
    std::vector<MathLib::Vec2> tvertices_;
    int num_tvertices_;

  
  private:
    
    
};

#endif /* __SHIP_H__ */