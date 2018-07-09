#ifndef DYNSOLVER_MATH_POLY_ROOT_H_
#define DYNSOLVER_MATH_POLY_ROOT_H_

#include <cstddef>

#include "math/complex.h"

namespace dynsolver {
namespace math {

// Represents the root of a polynomial. That is, a complex number with
// a multiplicity.
class poly_root {
private:
  complex root;
  size_t multiplicity;
  
public:
  poly_root(const complex& root, size_t multiplicity);

  const complex& get_root() const;
  size_t get_multiplicity() const;
};
} // namespace math
} // namespace dynsolver

#endif
