/* Copyright 2015 Marcos Vazquez. All rights reserved.
 *
 * Author: Marcos Vazquez <vazquezre@esat-alumni.es>
 *
 * File:   Ship.h
 * Represents the lunar module controlled by the player
 *
 */

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
#include "Vehicle.h"


class Ship: public Vehicle {
  public:
    Ship();
    Ship(const Ship& orig);
    virtual ~Ship();
    
    /**
     * Creates and stores a collection of points in a parametrized buffer
     * @param num_vertices      The number of vertices for the polygon
     * @param radius            Radius of the polygon
     * @param position_offset   An offset for the points to be placed with respect of 0,0
     * @param rotation_offset   A rotation offset, set clockwise
     * @param buffer            Where the points will be stored
     * @param counter           Am int pointer to hold the number of points of the polygon
     */
    void assignRegularPolygon(int num_vertices, float radius, MathLib::Vec2 position_offset, float rotation_offset, std::vector<MathLib::Vec2>& buffer, int* counter);
    
    ///Animates a particle effect for the exploding ship
    void explode();
    
    
    void update() override;
    void move() override;
    void drawThruster();
    void draw() override;
    void setPhysics() override;
    void removePhysics();
    
    float fuel_;
    bool thrusting_;
    
    bool landed_;
    bool crashed_;
    
    float explosion_time_;
    
    std::vector<MathLib::Vec2> cvertices_;
    int num_cvertices_;
    
    std::vector<MathLib::Vec2> tvertices_;
    int num_tvertices_;

  private:
};

#endif /* __SHIP_H__ */