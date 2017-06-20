#ifndef UTIL_GUARD
#define UTIL_GUARD
#include "math/vector_2d.h"

vector_2d<int> value_to_pixel(const vector_2d<double>&, const vector_2d<int>&,
  const vector_2d<double>&, const vector_2d<double>&);

vector_2d<double> pixel_to_value(const vector_2d<int>&, const vector_2d<int>&,
  const vector_2d<double>&, const vector_2d<double>&);

/*Returns the number of iterations [1, iterations) it took to leave the disk of radius 2
* or zero if the point is in the mandelbrot set.*/
unsigned long mandelbrot(const vector_2d<double>& point, unsigned long iterations);
#endif
