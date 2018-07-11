// This file is for a main executable that is used for ad hoc tests and GDB debugging.

#include <iostream>
#include <string>
#include <vector>
#include "math/polynomial.h"
#include "math/poly_matrix.h"
#include "math/poly_root.h"
#include "math/square_matrix.h"
#include "math/eigenvalue.h"

int main() {
  int size = 3;
  dynsolver::math::square_matrix mat(size);
  for(int i = 0; i != size; ++i) {
    for(int j = 0; j != size; ++j) {
      mat[i][j] = i + j;
    }
  }
  std::vector<dynsolver::math::eigenvalue> values(mat.find_eigenvalues());
  for(int i = 0; i != values.size(); ++i) {
    std::cout << values[i].get_value().get_root().to_string() << std::endl;
  }
  return 0;
}
