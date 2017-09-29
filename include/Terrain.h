/* Copyright 2016 Marcos Vazquez. All rights reserved.
 *
 * Author: Marcos Vazquez <vazquezre@esat-alumni.es>
 *
 * File:   Terrain.h
 * Represents a continuous terrain as a collection of 2D points
 *
 */
//#ifndef WIN32
//#define WIN32 1
//#endif

#ifndef __TERRAIN_H__
#define __TERRAIN_H__ 1

#include <vector>
#include <cmath>
#include <ctgmath>
#include "MathLib.h"
#include <MOMOS/math.h>
#include <MOMOS/time.h>
#include <chipmunk/chipmunk.h>

#include "config.h"



#include "stb_perlin.h"


class Terrain {
  public:
    
    ///Special spots where a Ship could take land
    typedef struct {
      int begin, end;
      int size;
      int points;
      std::vector<cpShape*> shapes;
    } LandingSpot;
    
    Terrain();
    Terrain(const Terrain& orig);
    virtual ~Terrain();

    ///Draws the terrain
    void draw();
    
    ///Generates the points and LandingSpots of a new terrain
    void generate();
    
    ///Creates the static segments conforming the physics simulation
    void createWalls();
    
    ///Recalculates the points to be shown on screen, starting by position_index_
    void scroll();
    
    ///Physics space for the simulation
    cpSpace* space_;
    
    ///The collection of LandingSpots
    std::vector<LandingSpot> landings_;
    
    ///The index of landings_ where the terrain drawing must begin
    int position_index_ = 0;
    ///Actual position of the first point on the current screen
    float position_screen_ = 0;
    
    ///Terrain points on screen
    float onscreen_point_width_;
    int num_onscreen_points_ = 500;
    
    ///Terrain points total
    int num_terrain_points_ = 1000;
    float terrain_point_width_;
    
  private:
    
    void createLandingSpot(int begin, int size);
      

    ///Holds the points that conform the terrain
    std::vector<MathLib::Point2> points_;
    
    
};

#endif /* __TERRAIN_H__ */