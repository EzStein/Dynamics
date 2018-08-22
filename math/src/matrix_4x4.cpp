#include "math/matrix_4x4.h"

namespace math {

matrix_4x4::matrix_4x4() : square_matrix(4, 0.0) {
  (*this)[0][0] = 1;
  (*this)[1][1] = 1;
  (*this)[2][2] = 1;
  (*this)[3][3] = 1;
}

matrix_4x4 matrix_4x4::operator*(const matrix_4x4& rhs) const {
  matrix_4x4 lhs(*this);
  square_matrix mat(lhs.square_matrix::operator*(rhs));
  matrix_4x4 ret;
  for(int i = 0; i != 4; ++i) {
    for(int j = 0; j != 4; ++j) {
      ret[i][j] = mat[i][j];
    }
  }
  return ret;
}

matrix_4x4 matrix_4x4::translation(double x, double y) {
  matrix_4x4 mat;
  mat[0][3] = x;
  mat[1][3] = y;
  return mat;
}

matrix_4x4 matrix_4x4::translation(double x, double y, double z) {
  matrix_4x4 mat;
  mat[0][3] = x;
  mat[1][3] = y;
  mat[2][3] = z;
  return mat;
}

matrix_4x4 matrix_4x4::scale(double x, double y, double z) {
  matrix_4x4 mat;
  mat[0][0] = x;
  mat[1][1] = y;
  mat[2][2] = z;
  return mat;
}

matrix_4x4 matrix_4x4::scale(double x, double y) {
  matrix_4x4 mat;
  mat[0][0] = x;
  mat[1][1] = y;
  return mat;
}

} // namespace math
