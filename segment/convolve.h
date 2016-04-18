/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

/* convolution */

#ifndef CONVOLVE_H
#define CONVOLVE_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <opencv2\opencv.hpp>

/* convolve src with mask.  dst is flipped! */
static void convolve_even(IplImage *src, IplImage *dst, 
			  std::vector<float> &mask) {
  int width = src->width;
  int height = src->height;
  int len = mask.size();

  for (int y = 0; y < height; y++) {
	  float* data = (float*)(src->imageData + y*src->widthStep);
	  float* dstData = (float*)(dst->imageData + y*dst->widthStep);
    for (int x = 0; x < width; x++) {
      float sum = mask[0] * data[x];
      for (int i = 1; i < len; i++) {
		sum += mask[i] * (data[std::max(x-i,0)] + data[std::min(x+i, width-1)]);
      }
	  dstData[x] = sum;
    }
  }
}

#endif
