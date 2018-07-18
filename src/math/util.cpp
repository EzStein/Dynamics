#include "util.h"

namespace dynsolver {
namespace math {

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
} // namespace math
} // namespace dynsolver
