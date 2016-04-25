/* Copyright 2015 Marcos Vazquez. All rights reserved.    
 * 
 * Author: Marcos Vazquez <vazquezre@esat-alumni.es>  
 * Created on November 23, 2015, 1:22 AM
 *
 * File:   Misc.h                                              
 * A static class holding common functions
 *
 */
//#ifndef WIN32
//#define WIN32 1
//#endif

#ifndef MISC_H
#define MISC_H 1

#include <stdio.h>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <ESAT/sprite.h>


class Misc {
  
  public:

    static ESAT::SpriteHandle GetSubImage(ESAT::SpriteHandle img, int x, int y, 
                                          int width, int height);
    static int random(int max);
    static int random(int min, int max);
    static int random_sign();
    
    
  private:
    Misc();
    Misc(const Misc& orig);
    ~Misc();
};

#endif /* MISC_H */