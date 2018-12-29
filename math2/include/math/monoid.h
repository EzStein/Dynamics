#ifndef DYNSOLVER_MATH_MONOID_H_
#define DYNSOLVER_MATH_MONOID_H_
#include "math/semigroup.h"

namespace dynsolver {
namespace math {

// In algebra, a monoid is a set with a closed, associative binary
// operater containing an identity element.
// The identity element is returned by a call to the
// function zero() which must return the same value each
// time.
template<class T> class monoid : public semigroup<T> {
 public:
  // Returns the zero or identity element of this monoid.
  // We require for all a
  // equal(plus(zero(), a), a)
  // and
  // equal(plus(a, zero()), a)
  virtual T zero() const = 0;
};
} // namespace math
} // namespace dynsolver
#endif
