#include "util.h"

namespace dynsolver {
namespace math {

// Returns the subvector formed by extracting in order the elements of vec
// given by the indices.
vector extract_vector(const vector& vec, const std::vector<int>& indices) {
  vector ret(indices.size());
  for(int i = 0; i != indices.size(); ++i) {
    ret[i] = vec[indices[i]];
  }
  return ret;
}

bool solve_linear(const square_matrix& matrix,
		  const vector& vec, vector* solution) {
  assert(vec.size() == matrix.size());
  math::matrix augmented = matrix::adjoin_by_row(matrix, vec);
  math::matrix left, right;
  augmented.rref();
  augmented.split_vertically(matrix.get_cols(), &left, &right);
  if(!left.is_identity()) return false;
  vector result(matrix.size());
  for(int i = 0; i != right.get_rows(); ++i) {
    result[i] = right[i][0];
  }
  *solution = result;
  return true;
}

// Computes the intersection between the two lines defined as,
// A1 to B1 and A2 to B2.
vector2d intersect(const vector2d& A1, const vector2d& A2,
		   const vector2d& B1, const vector2d& B2) {
  
  vector2d ret;
  vector2d vec;
  matrix_2x2 mat;
  mat[0][0] = A2.y() - A1.y();
  mat[0][1] = A1.x() - A2.x();
  mat[1][0] = B2.y() - B1.y();
  mat[2][1] = B1.x() - B2.x();
  vec[0] = A1.x()*A2.y() - A1.y() * A2.x();
  vec[1] = B1.x()*B2.y() - B1.y() * B2.x();
  
  bool success = solve_linear(mat, vec, &ret);
  assert(success);
  return ret;
}

// Determines whether the points A1, and A2 lie on either side of the
// line B1 to B2.
bool splits(const vector2d& A1, const vector2d& A2,
	    const vector2d& B1, const vector2d& B2) {
  double val1 =
    (A1.y() - B1.y()) * (B2.x() - B1.x()) - (A1.x() - B1.x())*(B2.y() - B1.y());
  double val2 =
    (A2.y() - B1.y()) * (B2.x() - B1.x()) - (A2.x() - B1.x())*(B2.y() - B1.y());
  return val1 * val2 <= 0;
}
} // namespace math
} // namespace dynsolver
