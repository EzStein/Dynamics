#include "util.h"
#include "matrix_2d.h"

vector_2d<double> pixelToValue(const vector_2d<int>& pixel, const vector_2d<int>& pixelBoundary,
  const vector_2d<double>& valueBoundaryTop, const vector_2d<double>& valueBoundaryBottom) {

  double scaling = (valueBoundaryBottom.x - valueBoundaryTop.x)/pixelBoundary.x;
  matrix_2d<double> mat(scaling, 0, 0, -scaling);
  return valueBoundaryTop + mat * vector_2d<double>(pixel);
}

vector_2d<int> valueToPixel(const vector_2d<double>& value, const vector_2d<int>& pixelBoundary,
  const vector_2d<double>& valueBoundaryTop, const vector_2d<double>& valueBoundaryBottom) {

  double scaling = (valueBoundaryBottom.x - valueBoundaryTop.x)/pixelBoundary.x;
  matrix_2d<double> mat(1/scaling, 0, 0, -1/scaling);
  return mat * (value - valueBoundaryTop);
}
