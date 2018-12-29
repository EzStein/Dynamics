#ifndef DYNSOLVER_MATH_RING_H_
#define DYNSOLVER_MATH_RING_H_
#include "math/eq.h"
#include "math/monoid.h"
#include "math/abelian_group.h"

namespace dynsolver {
namespace math {

// In algebra, a ring is a set with an two operators.
// One operator forms an abelian group (call it addition)
// and the other forms a monoid (call it multiplication).
// The multiplication operator distributes over the
// adition operator.
// Since a ring may be viewed as both a monoid and a
// group, we do not inherit from either (there is no
// natural choice).

template<class T> class ring : public eq<T> {
 public:
  // We require that the equal operators of the monoid and abelian_group
  // be the same. That is, for all a and b,
  // mult_op().equal(a, b) iff add_op().equal(a, b)
  //
  // We define equal on the ring to be the same as it is
  // for both the monoid and the group.
  
  // We require that distribution holds for all a, b and c:
  // equal(mult_op().plus(add_op().plus(a, b), c),
  // add_op().plus(mult_op().plus(a, c), mult_op().plus(b, c)))
  //
  // equal(mult_op().plus(c, add_op().plus(a, b)),
  // add_op().plus(mult_op().plus(c, a), mult_op().plus(c, b)))

  // Returns the multiplication operator monoid.
  virtual monoid<T> mult_op() const = 0;

  // Returns the additive operator as an abelian group.
  virtual abelian_group<T> add_op() const = 0;

  // The equal operator for this ring.
  virtual bool equal(const T&, const T&) const override final;

  // Equivalent to mult_op.plus
  T times(const T&, const T&) const;

  // Equivalent to add_op.plus
  T plus(const T&, const T&) const;

  // equivalent to add_op.add_inv
  T add_inv(const T&) const;

  // Equivalent to add_op.zero
  T zero() const;

  // Equivalent to mult_op.zero
  T one() const;
};

template<class T> bool ring<T>::equal(const T& a, const T& b) const {
  return this->mult_op.equal(a, b);
}

template<class T> T ring<T>::times(const T& a, const T& b) const {
  return this->mult_op.plus(a, b);
}

template<class T> T ring<T>::plus(const T& a, const T& b) const {
  return this->add_op().plus(a, b);
}

template<class T> T ring<T>::add_inv(const T& a) const {
  return this->add_op().add_inv(a);
}

template<class T> T ring<T>::zero() const {
  return this->add_op().zero();
}

template<class T> T ring<T>::one() const {
  return this->mult_op().zero();
}
} // namespace math
} // namespace dynsolver
#endif
