#ifndef UTIL_GUARD
#define UTIL_GUARD
#include <vector>
#include "math/static_vector.h"
#include "math/static_matrix.h"
#include "math/vector.h"
#include "compiler/front/driver.h"

namespace math {
  static_vector<int, 2> value_to_pixel(const static_vector<double, 2>&, const static_vector<int, 2>&,
    const static_vector<double, 2>&, const static_vector<double, 2>&);
  static_vector<double, 2> pixel_to_value(const static_vector<int, 2>&, const static_vector<int, 2>&,
    const static_vector<double, 2>&, const static_vector<double, 2>&);
  static_vector<double, 2> pixel_to_value(const static_vector<int, 2>&,
    const static_vector<double, 2>&, const static_vector<double, 2>&);
  static_vector<int, 2> value_to_pixel(const static_vector<double, 2>&,
    const static_vector<double, 2>&, const static_vector<double, 2>&);

  void euler(std::vector<double>& initVals,
    const std::vector<driver::double_func_t>& vecField, double inc,
    double* scratchSpace = nullptr);
}
#endif
