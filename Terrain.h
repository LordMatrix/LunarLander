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
    
    
    cpSpace* space_;
    
    std::vector<LandingSpot> landings_;
    
  private:
    
    typedef struct {
      MathLib::Point2 vertices[4];
    } Box;
    
    std::vector<Box> boxes_;
    int num_boxes_ = 100;
    float box_width_;
};

#endif /* __TERRAIN_H__ */