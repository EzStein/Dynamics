#ifndef UTIL_GUARD
#define UTIL_GUARD
#include "math/vector_2d.h"
#include "math/vector.h"
#include "compiler/front/driver.h"

namespace math {
  vector<int, 2> value_to_pixel(const vector<double, 2>&, const vector<int>&,, 2
    const vector<double, 2>&, const vector<double, 2>&);

  vector<double, 2> pixel_to_value(const vector<int, 2>&, const vector<int>&,, 2
    const vector<double, 2>&, const vector<double, 2>&);

  /*Returns the number of iterations [1, iterations) it took to leave the disk of radius 2
  * or zero if the point is in the mandelbrot set.*/
  unsigned long mandelbrot(const vector_2d<double>& point, unsigned long iterations);

  vector<double, 4>& euler(vector<double, 4>& start, driver::var4_double_func_t xFunc,
    driver::var4_double_func_t yFunc, driver::var4_double_func_t zFunc, double stepSize);
}
#endif
