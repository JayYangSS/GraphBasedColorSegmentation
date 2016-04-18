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

#include <cstdio>
#include <cstdlib>
#include <misc.h>
#include "segment-image.h"
#include <opencv2\opencv.hpp>

float sigma = 0.8;
float k = 500;
int min_size = 100;

int main(int argc, char **argv) {

  printf("loading input image.\n");
  IplImage *src = cvLoadImage("C:\\Users\\Yang\\Documents\\Visual Studio 2013\\Projects\\ColorSegGraphBased\\Img\\00013.ppm");
  int width = src->width;
  int height = src->height;	

  IplImage *input = cvCreateImage(cvSize(width, height), IPL_DEPTH_32F, src->nChannels);
  cvConvertScale(src, input,1.0/255);

  printf("processing\n");
  int num_ccs; 
  IplImage *seg = segment_image(input, sigma, k, min_size, &num_ccs);
  
  //show result
  cvShowImage("input", input);
  cvWaitKey(5);
  cvShowImage("Seg", seg);
  cvWaitKey(5);
  cvSaveImage("Img\\result.jpg", seg);

  printf("got %d components\n", num_ccs);
  printf("done! uff...thats hard work.\n");

  cvWaitKey();
  return 0;
}

