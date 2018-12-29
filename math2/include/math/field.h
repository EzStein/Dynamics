#ifndef DYNSOLVER_MATH_FIELD_H_
#define DYNSOLVER_MATH_FIELD_H_
#include "math/commutative_ring.h"

namespace dynsolver {
namespace math {

// In algebra, a field is a commutative ring in which each
// element except the identity has a multiplicative inverse.
// Note that a field does not contain an abelian group over
// multiplication since inversion is not defined for zero().
// Thus we do not override the mult_op to return a group.
template<class T> class field : public commutative_ring<T> {
 public:
  // Returns the multiplicative inverse of T. If for some a,
  // equal(a, zero()), then the behavior of this function is
  // undefined. We require that
  // equal(times(mult_inv(a), a), one())
  //
  // This class (and all the algebra classes) must be consistent
  // with the equal comparator. That is, for any a, and b
  // if equal(a, b) then equal(mult_inv(a), mult_inv(b)) and
  // That is, multiplicative inverses are not
  // representation dependent.
  virtual T mult_inv(const T&) const = 0;

  // A convenience method with default implementation.
  // Returns times(a, mult_inv(b)). Requires that
  // !equal(b, zero())
  T divide(const T& a, const T& b) const;
};

template<class T> T field<T>::divide(const T& a, const T& b) const {
  return this->times(a, this->mult_inv(b));
}
} // namespace math
} // namespace dynsolver
#endif
