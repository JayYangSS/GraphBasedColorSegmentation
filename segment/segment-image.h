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

#ifndef SEGMENT_IMAGE
#define SEGMENT_IMAGE

#include <cstdlib>
#include <misc.h>
#include "segment-graph.h"
#include <opencv2\opencv.hpp>

// random color
rgb random_rgb(){ 
  rgb c;
  double r;
  
  c.r = (uchar)rand();
  c.g = (uchar)rand();
  c.b = (uchar)rand();

  return c;
}

// dissimilarity measure between pixels
static inline float diff(IplImage *r, IplImage *g, IplImage *b,
			 int x1, int y1, int x2, int y2) {

	//attention: 访问float类型的像素元素时一定要将行指针转换为float*，否则会出错
	float tmp = ((float*)(r->imageData + y1*r->widthStep))[x1];
	float r_dist = ((float*)(r->imageData + y1*r->widthStep))[x1] - ((float*)(r->imageData + y2*r->widthStep))[x2];
	float g_dist = ((float*)(g->imageData + y1*g->widthStep))[x1] - ((float*)(g->imageData + y2*g->widthStep))[x2];
	float b_dist = ((float*)(b->imageData + y1*b->widthStep))[x1] - ((float*)(b->imageData + y2*b->widthStep))[x2];

	return 255*sqrt(square(r_dist) + square(g_dist) + square(b_dist));
}

/*
 * Segment an image
 *
 * Returns a color image representing the segmentation.
 *
 * input: image to segment.
 * sigma: to smooth the image.
 * c: constant for treshold function.
 * min_size: minimum component size (enforced by post-processing stage).
 * num_ccs: number of connected components in the segmentation.
 */
void segment_image(int min_size) {
	int width = input->width;
	int height = input->height;
	std::cout << "k:" << k << std::endl;
	std::cout << "min_size:" << min_size << std::endl;
	
	IplImage *r = cvCreateImage(cvSize(width, height), input->depth,1);
	IplImage *g = cvCreateImage(cvSize(width, height), input->depth, 1);
	IplImage *b = cvCreateImage(cvSize(width, height), input->depth, 1);
	cvSplit(input, b, g, r,NULL);

	IplImage *smooth_r = cvCreateImage(cvSize(width, height), input->depth, 1);
	IplImage *smooth_g = cvCreateImage(cvSize(width, height), input->depth, 1);
	IplImage *smooth_b = cvCreateImage(cvSize(width, height), input->depth, 1);

	sigma = sigmaInt*0.01;
	std::cout << "sigma:" << sigma << std::endl;
	cvSmooth(r, smooth_r, CV_GAUSSIAN, 5, 5, sigma, sigma);
	cvSmooth(g, smooth_g, CV_GAUSSIAN, 5, 5, sigma, sigma);
	cvSmooth(b, smooth_b, CV_GAUSSIAN, 5, 5, sigma, sigma);

    cvReleaseImage(&r);
    cvReleaseImage(&g);
    cvReleaseImage(&b);
 
  // build graph
  edge *edges = new edge[width*height*4];
  int num = 0;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {

      if (x < width-1) {
	edges[num].a = y * width + x;
	edges[num].b = y * width + (x+1);
	edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y);
	num++;
      }

      if (y < height-1) {
	edges[num].a = y * width + x;
	edges[num].b = (y+1) * width + x;
	edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x, y+1);
	num++;
      }

      if ((x < width-1) && (y < height-1)) {
	edges[num].a = y * width + x;
	edges[num].b = (y+1) * width + (x+1);
	edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y+1);
	num++;
      }

      if ((x < width-1) && (y > 0)) {
		edges[num].a = y * width + x;
		edges[num].b = (y-1) * width + (x+1);
		edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y-1);
		num++;
      }
    }
  }
  cvReleaseImage(&smooth_r);
  cvReleaseImage(&smooth_g);
  cvReleaseImage(&smooth_b);

  // segment
  universe *u = segment_graph(width*height, num, edges, k);
  
  // post process small components
  for (int i = 0; i < num; i++) {
    int a = u->find(edges[i].a);
    int b = u->find(edges[i].b);
    if ((a != b) && ((u->size(a) < min_size) || (u->size(b) < min_size)))
      u->join(a, b);
  }
  delete [] edges;
  num_ccs = u->num_sets();
  std::cout << "component number:" << num_ccs << std::endl;

  output = cvCreateImage(cvSize(width, height),IPL_DEPTH_8U,input->nChannels);

  // pick random colors for each component
  rgb *colors = new rgb[width*height];
  for (int i = 0; i < width*height; i++)
    colors[i] = random_rgb();
  
  for (int y = 0; y < height; y++) {
	  uchar* data = (uchar*)(output->imageData + y*output->widthStep);
    for (int x = 0; x < width; x++) {
      int comp = u->find(y * width + x);
      //imRef(output, x, y) = colors[comp];
	  data[3 * x] = colors[comp].b;
	  data[3 * x+1] = colors[comp].g;
	  data[3 * x+2] = colors[comp].r;
    }
  }  

  delete [] colors;  
  delete u;

  cvShowImage("Seg", output);
  cvWaitKey(5);

  //return output;
}

#endif
