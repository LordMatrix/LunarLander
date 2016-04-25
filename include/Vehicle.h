/* Copyright 2016 Marcos Vazquez. All rights reserved.
 *
 * Author: Marcos Vazquez <vazquezre@esat-alumni.es>
 *
 * File:   Vehicle.h
 * Represents a physics body controllable by the player.
 * This class is effectively abstract. It MUST be inherited and its methods must be overriden.
 *
 */
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

    virtual void update();
    virtual void move();
    virtual void draw();
    virtual void setPhysics();
    
    MathLib::Vec2 pos_;
    MathLib::Vec2 velocity_;
    
    double last_update_;
    
    cpSpace* space_;
    
    cpBody* physics_body_;
    cpShape* physics_shape_;
    
  private:
    
    
};

#endif /* __VEHICLE_H__ */