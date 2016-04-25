//#ifndef WIN32
//#define WIN32 1
//#endif

#ifndef __LRV_H__
#define __LRV_H__ 1

#include <vector>
#include <cmath>
#include <ctgmath>
#include "MathLib.h"
#include <ESAT/math.h>
#include <ESAT/time.h>
#include "ESAT_extra/chipmunk/chipmunk.h"
#include "config.h"
#include "Vehicle.h"

class LRV: public Vehicle {
  public:
    LRV();
    LRV(const LRV& orig);
    virtual ~LRV();
    
    void assignRectangle(std::vector<MathLib::Vec2>& buffer, int* counter);
    void update() override;
    void move() override;
    void draw() override;
    void setPhysics() override;
    
    
    std::vector<MathLib::Vec2> qvertices_;
    int num_qvertices_;

  
  private:
    
    
};

#endif /* __LRV_H__ */