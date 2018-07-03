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
  matrix copy(*this);
  double det = copy.rref();
  return det;
}

bool square_matrix::invertable() const {
  matrix augmented(matrix::adjoin_by_row(*this, identity_matrix(rows)));
  augmented.rref();
  matrix left, right;
  augmented.rref();
  augmented.split_vertically(rows, &left, &right);
  return left.is_identity();
}

bool square_matrix::invert() {
  matrix augmented(matrix::adjoin_by_row(*this, identity_matrix(rows)));
  matrix left, right;
  augmented.rref();
  augmented.split_vertically(rows, &left, &right);
  bool invertible = left.is_identity();
  if(invertible) {
    this->matrix::operator=(right);
  }
  return invertible;
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

// We shall use a simple (Non optimized) qr decomposition
// iteration to find the eigenvalues. Currently this works only,
// when all eigenvalues have distinct size.
std::vector<double> square_matrix::eigenvalues() const {
  static const double tolerance = 1e-10;
  int size = rows;
  matrix acc(*this);
  matrix q, r;
  bool terminate = false;
  while(!terminate) {
    acc.qr_decomposition(q, r);
    acc *= q;
    q.transpose();
    acc = q * acc;

    // Scan the sub diagonal
    terminate = true;
    for(int i = 0; i + 1 != size; ++size) {
      if(std::abs(acc[i][i + 1]) >= tolerance) {
	terminate = false;
      }
    }
  }
  std::vector<double> values;
  for(int i = 0; i != size; ++i) {
    values.push_back(acc[i][i]);
  }
  return values;
}

square_matrix square_matrix::operator+(const square_matrix& rhs) const {
  square_matrix mat(*this);
  mat += rhs;
  return mat;
}

square_matrix square_matrix::operator-(const square_matrix& rhs) const {
  square_matrix mat(*this);
  mat -= rhs;
  return mat;
}

square_matrix square_matrix::operator*(const square_matrix& rhs) const {
  square_matrix mat(*this);
  mat *= rhs;
  return mat;
}

square_matrix square_matrix::operator*(double scal) const {
  square_matrix mat(*this);
  mat *= scal;
  return mat;
}

square_matrix square_matrix::operator/(double scal) const {
  square_matrix mat(*this);
  mat /= scal;
  return mat;
}
} // namespace math
} // namespace dynslover
