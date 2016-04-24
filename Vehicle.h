//#ifndef WIN32
//#define WIN32 1
//#endif

#ifndef __VEHICLE_H__
#define __VEHICLE_H__ 1

#include <vector>
#include <cmath>
#include <ctgmath>
#include "MathLib.h"
#include <ESAT/math.h>
#include <ESAT/time.h>
#include "ESAT_extra/chipmunk/chipmunk.h"
#include "config.h"

class Vehicle {
  public:
    Vehicle();
    Vehicle(const Vehicle& orig);
    virtual ~Vehicle();
    
    void assignRectangle(std::vector<MathLib::Vec2>& buffer, int* counter);
    void update();
    void drive();
    void draw();
    void setPhysics();
    
    MathLib::Vec2 pos_;
    MathLib::Vec2 velocity_;
    
    double last_update_;
    
    cpSpace* space_;
    
    cpBody* physics_body_;
    cpShape* physics_shape_;
    
    std::vector<MathLib::Vec2> cvertices1_;
    std::vector<MathLib::Vec2> cvertices2_;
    int num_cvertices_;
    
    std::vector<MathLib::Vec2> qvertices_;
    int num_qvertices_;

  
  private:
    
    
};

#endif /* __VEHICLE_H__ */