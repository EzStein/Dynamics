#include "math/matrix_4x4.h"

namespace dynsolver {
namespace math {

matrix_4x4::matrix_4x4() : square_matrix(4, 0.0) {
  (*this)[0][0] = 1;
  (*this)[1][1] = 1;
  (*this)[2][2] = 1;
  (*this)[3][3] = 1;
}

matrix_4x4 operator*(matrix_4x4 lhs, const matrix_4x4& rhs) {
  square_matrix mat(square_matrix_ops::operator*(lhs, rhs));
  matrix_4x4 ret;
  for(int i = 0; i != 4; ++i) {
    for(int j = 0; j != 4; ++j) {
      ret[i][j] = mat[i][j];
    }
  }
  return ret;
}

} // namespace math
} // namespace dynsolver
