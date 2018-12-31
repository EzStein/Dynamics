///
/// \file commutative_ring.h
/// \author Ezra Stein
/// Interface class for commutative ring structure.
///
#ifndef DYNSOLVER_MATH_COMMUTATIVE_RING_H_
#define DYNSOLVER_MATH_COMMUTATIVE_RING_H_
#include "math/ring.h"

namespace dynsolver {
namespace math {

/// A \c commutative_ring is a \c ring in which the multiplication operation
/// is commutative.
/// \invariant See \link ring \endlink
/// \tparam T The type of elements on which the ring operates.
template<class T> class commutative_ring : public ring<T> {
 public:
  /// Retrieves the multiplicative monoid.
  /// This is a specialization of the ring::mult_op function
  /// which returns a
  /// \link commutative_monoid \endlink as a covariant return type.
  /// \invariant See \link ring \endlink
  virtual commutative_monoid<T> mult_op() const override = 0;
};

} // namespace math
} // namespace dynsolver
#endif
