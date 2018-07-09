// This file is for a main executable that is used for ad hoc tests and GDB debugging.

#include <iostream>
#include <string>
#include <vector>
#include "math/polynomial.h"
#include "math/poly_matrix.h"
int main() {
  dynsolver::math::polynomial poly1(std::vector<double>{6, 12, 23});
  dynsolver::math::polynomial divisor(std::vector<double>{-2, 32, 3, 3, 3, 3, 3, 3});
  dynsolver::math::division_ret ret(poly1.divide(divisor));
  //  std::cout << "(" << poly1.to_string() <<
  //    ")/(" << poly2.to_string() <<
  //    ") = (" << ret.quotient.to_string() << ") + (" <<
  //    ret.remainder.to_string() << ")/(" << poly2.to_string() <<
  //    ")" << std::endl;
  std::cout << (ret.quotient * divisor + ret.remainder).to_string() << " = " <<
    poly1.to_string() << std::endl;
  return 0;
}
