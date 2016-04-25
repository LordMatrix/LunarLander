//#ifndef WIN32
//#define WIN32 1
//#endif

#ifndef __TERRAIN_H__
#define __TERRAIN_H__ 1

#include <vector>
#include <cmath>
#include <ctgmath>
#include "MathLib.h"
#include <ESAT/math.h>
#include <ESAT/time.h>
#include "ESAT_extra/chipmunk/chipmunk.h"

#include "config.h"



#include "stb_perlin.h"


class Terrain {
  public:
    
    typedef struct {
      int begin, end;
      int size;
      int points;
      std::vector<cpShape*> shapes;
    } LandingSpot;
    
    Terrain();
    Terrain(const Terrain& orig);
    virtual ~Terrain();

    void draw();
    void generate();
    
    void createWalls();
    void scroll(float amount);
    
    cpSpace* space_;
    
    std::vector<LandingSpot> landings_;
    
    int position_index_ = 0;
    float position_screen_ = 0;
    float onscreen_point_width_;
    int num_onscreen_points_ = 500;
    int num_terrain_points_ = 1000;
    float terrain_point_width_;
    
  private:
    
    void createLandingSpot(int begin, int size);
      
    typedef struct {
      MathLib::Point2 vertices[4];
    } Box;
    
    std::vector<MathLib::Point2> points_;
    
    
};

#endif /* __TERRAIN_H__ */