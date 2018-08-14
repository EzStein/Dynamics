// This file is for a main executable that is used for ad hoc tests and GDB debugging.

#include <iostream>
#include <string>
#include <vector>
#include "math/polynomial.h"
#include "math/poly_matrix.h"
#include "math/poly_root.h"
#include "math/square_matrix.h"
#include "math/eigenvalue.h"
#include "compiler/function.h"
#include "math/util.h"

using namespace dynsolver;

double val(const double* arr) {
  return arr[0] * arr[0]*arr[0];
}

double deriv(const double* arr) {
  return 3 * arr[0] * arr[0];
}

int main() {
  for(double i = -3; i <= 3; i += 0.1) {
    math::vector in(1, i);
    double computed = math::derivative(val, in, 0);
    double expected = deriv(in.data());
    std::cout << "Expected Derivative: " << expected << std::endl;
    std::cout << "Computed Derivative: " << computed << std::endl;
  }
  return 0;
}
