#ifndef DYNSOLVER_MATH_COMMUTATIVE_RING_H_
#define DYNSOLVER_MATH_COMMUTATIVE_RING_H_
#include "math/ring.h"

namespace dynsolver {
namespace math {

// A commutative ring is a ring in which the multiplication operation
// is commutative.
template<class T> class commutative_ring : public ring<T> {
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
  virtual commutative_monoid<T> mult_op() const override = 0;
};

} // namespace math
} // namespace dynsolver
#endif
