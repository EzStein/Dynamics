#include "math/square_matrix.h"

#include <cassert>
#include <cmath>
#include "math/poly_matrix.h"
#include "math/polynomial.h"
#include "math/eigenvalue.h"
#include "math/util.h"

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
/*std::vector<double> square_matrix::eigenvalues() const {
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
  }*/

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

std::vector<eigenvalue> square_matrix::find_eigenvalues() const {
  const double tolerance = 1e-30;
  if(size() == 2) {
    double a = 1;
    double b = -(*this)[0][0] - (*this)[1][1];
    double c = (*this)[0][0] * (*this)[1][1] - (*this)[1][0] * (*this)[0][1];
    double discriminant = b*b - 4 * a * c;
    if(std::abs(discriminant) < tolerance) {
      // Double root
      double root = -b / 2 * a;
      std::vector<vector>
	basis((*this - identity_matrix(2) * root).null_space());
      std::vector<eigenvalue> ret;
      ret.push_back(eigenvalue(poly_root(complex(root, 0), 2), basis));
      return ret;
    } else if(discriminant > 0) {
      // Real case distinct
      double val1 = (-b + std::sqrt(discriminant)) / 2 * a;
      double val2 = (-b - std::sqrt(discriminant)) / 2 * a;
      std::vector<vector>
	basis1((*this - identity_matrix(2) * val1).null_space());
      std::vector<vector>
	basis2((*this - identity_matrix(2) * val2).null_space());
      assert(basis1.size() == 1);
      assert(basis2.size() == 1);
      std::vector<eigenvalue> ret;
      std::vector<vector> vectors;
      vectors.push_back(basis1[0]);
      ret.push_back(eigenvalue(poly_root(complex(val1, 0), 1), vectors));
      vectors.clear();
      vectors.push_back(basis2[0]);
      ret.push_back(eigenvalue(poly_root(complex(val2, 0), 1), vectors));
      return ret;
    } else {
      // Complex case distinct
      complex discriminantComplex(discriminant, 0);
      discriminantComplex.sqrt();
      complex val1 = (complex(-b, 0) + discriminantComplex) / complex(2 * a, 0);
      complex val2 = (complex(-b, 0) - discriminantComplex) / complex(2 * a, 0);
      std::vector<eigenvalue> ret;
      ret.push_back(eigenvalue(poly_root(val1, 1), std::vector<vector>()));
      ret.push_back(eigenvalue(poly_root(val2, 1), std::vector<vector>()));
      return ret;
    }
  } else {
    return std::vector<eigenvalue>();
  }
  
  /*  const double tolerance = 1e-10;
  
  // We first perform iterative QR decomposition
  square_matrix mat(*this);
  const int maxIterations = 100;
  for(int i = 0; i != maxIterations; ++i) {
    square_matrix q;
    square_matrix r;
    mat.qr_decomposition(q, r);
    mat = r * q;
    // Check for termination;
    
  }
  // For each submatrix we compute a set of eigenvalues.
  std::vector<eigenvalue> values;
  int start = 0;
  while(start != mat.size()) {
    int r = start;
    while(std::abs(mat[r][start]) >= tolerance && r != mat.size()) {
      ++r;
    }
    poly_matrix sub(r - start);
    for(int i = start; i != r; ++i) {
      for(int j = start; j != r; ++j) {
	if(i == j) {
	  sub[i][j] = polynomial(std::vector<double>{mat[i][j], -1});
	} else {
	  sub[i][j] = polynomial(std::vector<double>{mat[i][j]});
	}
      }
    }
    const poly_matrix& subConst(sub);
    polynomial characteristic(subConst.determinant());
    std::vector<poly_root> roots(characteristic.find_roots());
    for(std::vector<poly_root>::const_iterator root = roots.begin();
	root != roots.end(); ++root) {
      std::vector<math::vector> vectors;
      values.push_back(eigenvalue(*root, vectors));
    }
    start = r;
  }
  return values;*/
}
} // namespace math
} // namespace dynslover
