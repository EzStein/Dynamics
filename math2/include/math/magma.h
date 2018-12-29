#ifndef DYNSOLVER_MATH_MAGMA_H_
#define DYNSOLVER_MATH_MAGMA_H_
#include "math/eq.h"

namespace dynsolver {
namespace math {

// In algebra a magma is a set with a binary operation that
// satisfies closure. It is the simplest of all algebraic structures,
// that involve a binary operator.
//
// We denote the operator as magma:plus.
//
// This class (and all the algebra classes) must be consistent
// with the equal comparator. That is, for any a, b, and c
// if equal(a, b) then equal(plus(a, c), plus(b, c)) and
// equal(plus(c, a), plus(c, b)). That is, addition is
// not representation dependent.
template<class T> class magma : public eq<T> {
 public:
  // Returns the sum of the two elements. Closure is enforced
  // by the type system.
  virtual T plus(const T&, const T&) const = 0;
};

} // namespace math
} // namespace dynsolver

#endif
