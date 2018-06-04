#include "math/square_matrix.h"

#include <cassert>

namespace dynsolver {
namespace math {

square_matrix::square_matrix(int size) : matrix(size, size) { }
square_matrix::square_matrix(int size, double value) : matrix(size, size, value) { }

int square_matrix::size() const {
  return rows;
}

double square_matrix::determinant() const {
  double det;
  matrix copy(*this);
  copy.rref(det);
  return det;
}

bool square_matrix::invertable() const {
  int size = rows;
  matrix augmented = matrix::adjoin_by_row(*this, identity_matrix(size));
  augmented.rref();
  augmented.set_ones();
  augmented.set_zeros();
  // We check if the matrix row reduced to the identiy.
  for(int row = 0; row != size; ++row) {
    for(int col = 0; col != size; ++col) {
      double val = augmented[row][col];
      if(val != (row == col?1:0)) return false;
    }
  }
  return true;
}

bool square_matrix::invert() {
  int size = rows;
  matrix augmented = matrix::adjoin_by_row(*this, identity_matrix(size));
  augmented.rref();
  augmented.set_ones();
  augmented.set_zeros();

  // We check if the matrix row reduced to the identiy.
  for(int row = 0; row != size; ++row) {
    for(int col = 0; col != size; ++col) {
      double val = augmented[row][col];
      if(val != (row == col?1:0)) return false;
    }
  }
  for(int row = 0; row != size; ++row) {
    for(int col = 0; col != size; ++col) {
      (*this)[row][col] = augmented[row][col+size];
    }
  }
  return true;
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

// These functions perform the arithmatic in the expected way on square matrices.
square_matrix operator+(square_matrix lhs, const square_matrix& rhs) {
  lhs += rhs;
  return lhs;
}
square_matrix operator-(square_matrix lhs, const square_matrix& rhs) {
  lhs -= rhs;
  return lhs;
}
square_matrix operator*(square_matrix lhs, const square_matrix& rhs) {
  lhs *= rhs;
  return lhs;
}
square_matrix operator*(double scalar, square_matrix mat) {
  mat *= scalar;
  return mat;
}
square_matrix operator*(square_matrix mat, double scalar) {
  mat *= scalar;
  return mat;
}
square_matrix operator/(square_matrix mat, double scalar) {
  mat /= scalar;
  return mat;
}
} //namespace math
} //namespace dynslover
