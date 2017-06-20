#include "math/util.h"
#include "math/matrix_2d.h"

vector_2d<double> pixel_to_value(const vector_2d<int>& pixel, const vector_2d<int>& pixelBoundary,
  const vector_2d<double>& valueBoundaryTop, const vector_2d<double>& valueBoundaryBottom) {

  double scaling = (valueBoundaryBottom.x - valueBoundaryTop.x)/pixelBoundary.x;
  matrix_2d<double> mat(scaling, 0, 0, -scaling);
  return valueBoundaryTop + mat * vector_2d<double>(pixel);
}

vector_2d<int> value_to_pixel(const vector_2d<double>& value, const vector_2d<int>& pixelBoundary,
  const vector_2d<double>& valueBoundaryTop, const vector_2d<double>& valueBoundaryBottom) {

  double scaling = (valueBoundaryBottom.x - valueBoundaryTop.x)/pixelBoundary.x;
  matrix_2d<double> mat(1/scaling, 0, 0, -1/scaling);
  return vector_2d<int>(mat * (value - valueBoundaryTop));
}

unsigned long mandelbrot(const vector_2d<double>& point, unsigned long iterations) {
  double cx = point.x;
  double cy = point.y;

  for (unsigned long i = 1; i != iterations; ++i) {
    double cxSquared = cx * cx;
    double cySquared = cy * cy;
    if(cxSquared + cySquared > 4.0)
      return i;


    double cxTimesCy = cx * cy;
    cx = cxSquared - cySquared + point.x;
    cy = 2 * cxTimesCy + point.y;
  }
  return 0;
}
