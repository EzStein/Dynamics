#include "math/square_matrix.h"

#include <cassert>

namespace dynsolver {
namespace math {
square_matrix::square_matrix() : matrix() { }
square_matrix::square_matrix(int size) : matrix(size, size) { }
square_matrix::square_matrix(int size, double value)
  : matrix(size, size, value) { }

int square_matrix::size() const {
  return rows;
}

double square_matrix::determinant() const {
  double det;
  matrix copy(*this);
  copy.rref(&det);
  return det;
}

bool square_matrix::invertable() const {
  matrix augmented(matrix::adjoin_by_row(*this, identity_matrix(rows)));
  augmented.rref();
  matrix left, right;
  augmented.rref().split_vertically(rows, &left, &right);
  return left.is_identity();
}

square_matrix& square_matrix::invert(bool* success) {
  matrix augmented(matrix::adjoin_by_row(*this, identity_matrix(rows)));
  matrix left, right;
  augmented.rref().split_vertically(rows, &left, &right);
  bool invertible = left.is_identity();
  if(invertible) {
    this->matrix::operator=(right);
  }
  if(success != nullptr) {
    *success = invertible;
  }
  return *this;
}

square_matrix square_matrix::identity_matrix(int size) {
  square_matrix retMat(size);
  for(int r = 0; r != size; ++r) {
    for(int c = 0; c != size; ++c) {
      retMat[r][c] = (r == c)?1:0;
    }
  }
  return retMat;
}

square_matrix& square_matrix::operator+=(const square_matrix& other) {
  matrix::operator+=(other);
  return *this;
}
square_matrix& square_matrix::operator-=(const square_matrix& other) {
  matrix::operator-=(other);
  return *this;
}
square_matrix& square_matrix::operator*=(const square_matrix& other) {
  matrix::operator*=(other);
  return *this;
}
square_matrix& square_matrix::operator*=(double scal) {
  matrix::operator*=(scal);
  return *this;
}
square_matrix& square_matrix::operator/=(double scal) {
  matrix::operator/=(scal);
  return *this;
}

namespace square_matrix_ops {
// These functions perform the arithmatic in the expected way on square matrices.
square_matrix operator+(square_matrix lhs, const square_matrix& rhs) {
  return lhs += rhs;
}
square_matrix operator-(square_matrix lhs, const square_matrix& rhs) {
  return lhs -= rhs;
}
square_matrix operator*(square_matrix lhs, const square_matrix& rhs) {
  return lhs *= rhs;
}
square_matrix operator*(double scalar, square_matrix mat) {
  return mat *= scalar;
}
square_matrix operator*(square_matrix mat, double scalar) {
  return mat *= scalar;
}
square_matrix operator/(square_matrix mat, double scalar) {
  return mat /= scalar;
}
} // namespace square_matrix_ops
} // namespace math
} // namespace dynslover
