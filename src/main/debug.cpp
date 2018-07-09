// This file is for a main executable that is used for ad hoc tests and GDB debugging.

#include <iostream>
#include <string>
#include <vector>
#include "math/polynomial.h"
#include "math/poly_matrix.h"
int main() {
  dynsolver::math::polynomial poly1(std::vector<double>{5, 1});
  dynsolver::math::polynomial poly2(std::vector<double>{2, 2});
  dynsolver::math::polynomial poly3(std::vector<double>{3});
  dynsolver::math::polynomial poly4(std::vector<double>{4});

  dynsolver::math::poly_matrix mat(2);
  mat[0][0] = poly1;
  mat[0][1] = poly2;
  mat[1][0] = poly3;
  mat[1][1] = poly4;

  const dynsolver::math::poly_matrix& matRef(mat);
  std::cout << matRef.determinant().to_string() << std::endl;
  return 0;
}
