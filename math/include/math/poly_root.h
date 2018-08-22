#ifndef MATH_POLY_ROOT_H_
#define MATH_POLY_ROOT_H_

#include <cstddef>

#include "math/complex.h"

namespace math {

// Represents the root of a polynomial. That is, a complex number with
// a multiplicity.
class poly_root {
private:
  complex root;
  size_t multiplicity;
  
public:
  poly_root(const complex& root, size_t multiplicity);

  // Getters and setters.
  const complex& get_root() const;
  size_t get_multiplicity() const;
  void set_multiplicity(size_t);
};
} // namespace math

#endif
