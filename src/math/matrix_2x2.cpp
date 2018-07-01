#include "math/matrix_2x2.h"

#include "math/vector2d.h"

namespace dynsolver {
namespace math {

matrix_2x2::matrix_2x2() : square_matrix(2, 0.0) {
  (*this)[0][0] = 1;
  (*this)[1][1] = 1;
}

matrix_2x2 operator*(matrix_2x2 lhs, const matrix_2x2& rhs) {
  square_matrix mat(square_matrix_ops::operator*(lhs, rhs));
  matrix_2x2 ret;
  for(int i = 0; i != 2; ++i) {
    for(int j = 0; j != 2; ++j) {
      ret[i][j] = mat[i][j];
    }
  }
  return ret;
}

vector2d operator*(matrix_2x2 lhs, const vector2d& rhs) {
  vector vec(vector_ops::operator*(lhs, rhs));
  vector2d ret(0, 0);
  for(int i = 0; i != 2; ++i) {
    ret[i] = vec[i];
  }
  return ret;
}

} // namespace math
} // namespace dynsolver
