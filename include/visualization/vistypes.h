/*
 * vistypes.h
 *
 *  Created on: May 18, 2012
 *      Author: derrick
 */

#ifndef VISTYPES_H_
#define VISTYPES_H_

#include <vector>
#include <unistd.h>
#include <cstdlib>

typedef struct {
  size_t width;
  size_t height;
  size_t channels;
  size_t rowspan;
  double *im;
} VisImage;

inline size_t VisImIndex(VisImage *image, size_t x, size_t y) {
  return image->channels * x + image->rowspan * y;
}

inline VisImage * VisCreateImage(size_t width, size_t height, size_t channels) {
  VisImage *image = new VisImage();
  image->im = (double*)malloc(width * height * channels * sizeof(double));
  image->width = width;
  image->height = height;
  image->channels = channels;
  image->rowspan = channels * width;

  return image;
}

inline void VisSetPx(VisImage* image, size_t x, size_t y, double* vals) {
  double* px = &image->im[VisImIndex(image, x, y)];
  for (size_t i = 0; i < image->channels; ++i) {
    px[i] = vals[i];
  }
}

#endif /* VISTYPES_H_ */
