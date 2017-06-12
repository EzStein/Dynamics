#ifndef UTIL_GUARD
#define UTIL_GUARD
#include "vector_2d.h"

vector_2d<int> valueToPixel(const vector_2d<double>&, const vector_2d<int>&,
  const vector_2d<double>&, const vector_2d<double>&);

vector_2d<double> pixelToValue(const vector_2d<int>&, const vector_2d<int>&,
  const vector_2d<double>&, const vector_2d<double>&);

#endif
