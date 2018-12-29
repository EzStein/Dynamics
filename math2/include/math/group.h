#ifndef DYNSOLVER_MATH_GROUP_H_
#define DYNSOLVER_MATH_GROUP_H_
#include "math/monoid.h"

namespace dynsolver {
namespace math {

// In algebra a group is a set endowed with a closed associative binary
// operation with identity that also contains an inverse for each element.
template<class T> class group : public monoid<T> {
 public:
  // Returns the inverse of the provided element.
  // Requires that for all a,
  // equal(plus(add_inv(a), a), zero)
  // and
  // equal(plus(a, add_inv(a)), zero)
  //
  // This class (and all the algebra classes) must be consistent
  // with the equal comparator. That is, for any a, and b
  // if equal(a, b) then equal(add_inv(a), add_inv(b))
  // That is, addititive inverses are
  // not representation dependent.
  virtual T add_inv(const T&) const = 0;

  // A convenience function which has a default implementation.
  // Returns plus(a, inverse(b))
  T minus(const T& a, const T& b) const;
};

template<class T> T group<T>::minus(const T& a, const T& b) const {
  return this->plus(a, this->add_inv(b));
}
} // namespace math
} // namespace dynsolver
#endif
