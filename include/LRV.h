/* Copyright 2016 Marcos Vazquez. All rights reserved.
 *
 * Author: Marcos Vazquez <vazquezre@esat-alumni.es>
 *
 * File:   Ship.h
 * Represents the Lunar Roving Vehicle controlled by the player
 *
 */
//#ifndef WIN32
//#define WIN32 1
//#endif

#ifndef __LRV_H__
#define __LRV_H__ 1

#include <vector>
#include <cmath>
#include <ctgmath>
#include "MathLib.h"
#include <MOMOS/math.h>
#include <MOMOS/time.h>
#include <chipmunk/chipmunk.h>
#include "config.h"
#include "Vehicle.h"

class LRV: public Vehicle {
  public:
    LRV();
    LRV(const LRV& orig);
    virtual ~LRV();
    
    /**
     * Creates and stores a rectangle in a parametrized buffer
     * @param buffer            Where the points will be stored
     * @param counter           Am int pointer to hold the number of points of the polygon
     */
    void assignRectangle(std::vector<MathLib::Vec2>& buffer, int* counter);
    
    void update() override;
    
    ///Listens to player input and updates vehicle position & acceleration
    void move() override;
    
    ///Draws the vehicle at its position & rotation
    void draw() override;
    
    ///Initializes physics simulation for this ship
    void setPhysics() override;
    
    ///Vertices holding the quad geometry
    std::vector<MathLib::Vec2> qvertices_;
    int num_qvertices_;

    bool locked_;
    
  private:
    
};

#endif /* __LRV_H__ */