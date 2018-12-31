///
/// \file monoid.h
/// \author Ezra Stein
/// Definition of the monoid interface.
///
#ifndef DYNSOLVER_MATH_MONOID_H_
#define DYNSOLVER_MATH_MONOID_H_
#include "math/semigroup.h"

namespace dynsolver {
namespace math {

/// A \c monoid is a set with a closed, associative binary
/// operater containing an identity element. One may think of
/// this monoid interface as a \link semigroup \endlink
/// with an identity element.
/// \tparam T The type of the objects on which this monoid operates.
template<class T> class monoid : public semigroup<T> {
 public:
  /// Returns the zero or identity element of this monoid.
  /// \return The identity element of the monoid. It must return
  /// equivalent values on each call with respect to the
  /// eq::equal operator that the \c monoid inherits.
  /// \invariant For all \c a we require that
  /// \code equal(plus(zero(), a), a) \endcode
  /// and
  /// \code equal(plus(a, zero()), a) \endcode
  virtual T zero() const = 0;
};
} // namespace math
} // namespace dynsolver
#endif
