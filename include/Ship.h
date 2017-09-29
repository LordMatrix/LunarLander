/* Copyright 2016 Marcos Vazquez. All rights reserved.
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
#include <MOMOS/math.h>
#include <MOMOS/time.h>
#include <chipmunk/chipmunk.h>
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
    
    ///Listens to player input and updates ship position & acceleration
    void move() override;
    
    ///Draws a fire effect on the back of the ship
    void drawThruster();
    
    ///Draws the ship at its position & rotation
    void draw() override;
    
    ///Initializes physics simulation for this ship
    void setPhysics() override;
    
    ///Removes the ship body from the physics simulation
    void removePhysics();
    
    ///Amount of fuel left
    float fuel_;
    ///Whether or not the ship is burning fuel
    bool thrusting_;
    
    ///Game status for this ship
    bool landed_;
    bool crashed_;
    
    ///Time elapsed since the crashing explosion began
    float explosion_time_;
    
    ///Vertices holding the circle geometry
    std::vector<MathLib::Vec2> cvertices_;
    int num_cvertices_;
    
    ///Vertices holding the triangle geometry
    std::vector<MathLib::Vec2> tvertices_;
    int num_tvertices_;

  private:
};

#endif /* __SHIP_H__ */