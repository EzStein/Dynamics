#include "math/util.h"
#include "math/matrix.h"
namespace math {
  vector<double, 2> pixel_to_value(const vector<int, 2>& pixel, const vector<int, 2>& canvasSize,
    const vector<double, 2>& valueBoundaryTopLeft, const vector<double, 2>& valueBoundaryBottomRight) {

    double scaling = (valueBoundaryBottomRight[0] - valueBoundaryTopLeft[0])/canvasSize[0];
    matrix<double, 2, 2> mat;
    mat[0][0] = scaling;
    mat[0][1] = 0;
    mat[1][0] = 0;
    mat[1][1] = -scaling;
    return valueBoundaryTopLeft + mat * vector<double, 2>(pixel);
  }

  vector<int, 2> value_to_pixel(const vector<double, 2>& value, const vector<int, 2>& canvasSize,
    const vector<double, 2>& valueBoundaryTopLeft, const vector<double, 2>& valueBoundaryBottomRight) {

    double scaling = (valueBoundaryBottomRight[0] - valueBoundaryTopLeft[0])/canvasSize[0];
    matrix<double, 2, 2> mat;
    mat[0][0] = 1.0/scaling;
    mat[0][1] = 0;
    mat[1][0] = 0;
    mat[1][1] = -1.0/scaling;
    return vector<int, 2>(mat * (value - valueBoundaryTopLeft));
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

  vector<double, 4>& euler(vector<double, 4>& start,
    driver::var4_double_func_t xFunc, driver::var4_double_func_t yFunc, driver::var4_double_func_t zFunc, double inc) {
    double xInc = inc*xFunc(start[0], start[1], start[2], start[3]);
    double yInc = inc*yFunc(start[0], start[1], start[2], start[3]);
    double zInc = inc*zFunc(start[0], start[1], start[2], start[3]);
    start[0] += inc;
    start[1] += xInc;
    start[2] += yInc;
    start[3] += zInc;
    return start;
  }
}
