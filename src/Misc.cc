/* 
 * File:   Misc.cc
 * Author: Marcos Vazquez
 * 
 * Created on November 23, 2015, 1:22 AM
 * 
 * General purpose and misc functions
 */

#include "../include/Misc.h"


Misc::Misc() {
}

Misc::Misc(const Misc& orig) {
}

Misc::~Misc() {
}

/** @brief Extracts a part of an image
 *  This method scans an image pixel by pixel, creates a memory map of the pixels
 *  and composes a new image using a subset of it.
 *
 *  @return MOMOS::SpriteHandle The resulting sub-image
 *  @param MOMOS::SpriteHandle A big image to be trimmed
 *  @param x Starting X pixel (top left)
 *  @param y Starting Y pixel (top left)
 *  @param width Number of pixels wide of the new image
 *  @param height Number of pixels height of the new image
 */
MOMOS::SpriteHandle Misc::GetSubImage(MOMOS::SpriteHandle img, int x, int y, int width, int height) {
  //Iterate through every single pixel and add it to a pixel buffer
  //unsigned char pixel_buffer[height*width*4];
  std::vector<unsigned char> pixel_buffer(height*width*4);;
  int buffer_index=0;

  for (int h=y; h<y+height; h++) {
    for (int w=x; w<x+width; w++) {
      unsigned char outRGBA[4];
      MOMOS::SpriteGetPixel(img, w, h, outRGBA);
      pixel_buffer[buffer_index+0] = outRGBA[0];
      pixel_buffer[buffer_index+1] = outRGBA[1];
      pixel_buffer[buffer_index+2] = outRGBA[2];
      pixel_buffer[buffer_index+3] = outRGBA[3];
      buffer_index += 4;
    }
  }
  
  unsigned char* pixels_array = &pixel_buffer[0];
  
  return MOMOS::SpriteFromMemory(width, height, pixels_array);
}

/** @brief  Calculates a random number between 0 and <limit>
 * 
 *  @return A random number between 0 and <limit>
 *  @param  limit an integer limit
 */
int Misc::random(int max) {
  if (max<=0)
    max = 1;
  
  return (rand() % max);
}

int Misc::random(int min, int max) {
	return (random(max - min) + min);
}

/** @brief  Randomly returns 1 or -1
 *  @return Randomly returns 1 or -1
 */
int Misc::random_sign() {
	if (random(2) == 1)
		return 1;
	else
		return -1;
}