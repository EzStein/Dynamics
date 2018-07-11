#include "math/eigenvalue.h"

namespace dynsolver {
namespace math {

eigenvalue::eigenvalue(const poly_root& value,
		       const std::vector<math::vector>& vectors)
  : value(value), vectors(vectors) { }

const poly_root& eigenvalue::get_value() const {
  return value;
}

const std::vector<math::vector>& eigenvalue::get_vectors() const {
  return vectors;
}

} // namespace math
} // namespace dynsolver
