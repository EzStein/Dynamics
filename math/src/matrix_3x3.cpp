#include "math/matrix_3x3.h"

#include "math/matrix_4x4.h"
#include "math/vector3d.h"

namespace math {

matrix_3x3::matrix_3x3() : square_matrix(3, 0.0) {
  (*this)[0][0] = 1;
  (*this)[1][1] = 1;
  (*this)[2][2] = 1;
}

matrix_4x4 matrix_3x3::augment_to_4x4() const {
  matrix_4x4 ret;
  for(int i = 0; i != 3; ++i) {
    for(int j = 0; j != 3; ++j) {
      ret[i][j] = (*this)[i][j];
    }
  }
  return ret;
}

matrix_3x3 matrix_3x3::operator*(const matrix_3x3& rhs) const {
  matrix_3x3 lhs(*this);
  square_matrix mat(lhs.square_matrix::operator*(rhs));
  matrix_3x3 ret;
  for(int i = 0; i != 3; ++i) {
    for(int j = 0; j != 3; ++j) {
      ret[i][j] = mat[i][j];
    }
  }
  return ret;
}

vector3d matrix_3x3::operator*(const vector3d& rhs) const {
  matrix_3x3 lhs(*this);
  vector vec(lhs.matrix::operator*(rhs));
  vector3d ret(0, 0, 0);
  for(int i = 0; i != 3; ++i) {
    ret[i] = vec[i];
  }
  return ret;
}

} // namespace math
