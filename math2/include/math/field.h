///
/// \file field.h
/// \author Ezra Stein
/// Interface class for a field structure.
///
#ifndef DYNSOLVER_MATH_FIELD_H_
#define DYNSOLVER_MATH_FIELD_H_
#include "math/commutative_ring.h"

namespace dynsolver {
namespace math {

/// A \c field is a \link commutative_ring \endlink in which each
/// element except the \link ring::zero additive identity \endlink
/// has a multiplicative inverse.
/// \tparam T The type of elements on which this field operates.
template<class T> class field : public commutative_ring<T> {
 public:
  /// Computes the multiplicative inverse of the provided element \p a.
  /// \param a The element for which this function finds an inverse.
  /// If \code equal(zero(), a) \endcode the behavior of this
  /// function is undefined.
  /// \return The multiplicative inverse of \p a if \p a is not
  /// equal to the additive identity.
  /// \invariant For all \p a, the multiplicative inverse is indeed
  /// a left inverse (and therefor right),
  /// \code equal(times(mult_inv(a), a), one()) \endcode
  /// Additionally, the inverse function must be consistent with the
  /// inherited equality operator.
  /// That is, for all \p a and \p b, if
  /// \code equal(a, b)\endcode then
  /// \code equal(mult_inv(a), mult_inv(b)) \endcode
  /// In particular, multiplicative inverses are not
  /// representation dependent.
  /// \warning If \code equal(zero(), a) \endcode the behavior of this
  /// function is undefined.
  virtual T mult_inv(const T& a) const = 0;

  /// Performs a division of \p a by \p b. That is, this function
  /// multiplies \p a by the inverse of \p b.
  /// \param a The element to be divided.
  /// \param b The divisor which will divide \p a. If 
  /// \code equal(zero(), b) \endcode the behavior of this
  /// function is undefined.
  /// \return The result of \p a divided by \p b.
  /// \code times(a, mult_inv(b)) \endcode
  /// \warning If \p b is equal to the additive identity, that is, if
  /// \code equal(zero(), b) \endcode the behavior of this
  /// function is undefined.
  T divide(const T& a, const T& b) const;
};

template<class T> T field<T>::divide(const T& a, const T& b) const {
  return this->times(a, this->mult_inv(b));
}
} // namespace math
} // namespace dynsolver
#endif
