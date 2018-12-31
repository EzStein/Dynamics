///
/// \file group.h
/// \author Ezra Stein
/// Interface class for group structures.
///
#ifndef DYNSOLVER_MATH_GROUP_H_
#define DYNSOLVER_MATH_GROUP_H_
#include "math/monoid.h"

namespace dynsolver {
namespace math {
/// A \c group is a set endowed with a closed associative binary
/// operator with identity that also contains an inverse for each
/// element. It may be thought of as a \link monoid \endlink with
/// inverses.
/// \tparam T The type of the objects on which the group operates.
template<class T> class group : public monoid<T> {
 public:
  /// Returns the inverse of the input. We adopt the convention
  /// that this is the additive inverse with respect to the inherited
  /// magma::plus operation.
  /// \param a Any element of type \c T.
  /// \return The inverse of \p a with respect to the
  /// magma::plus operator.
  /// \invariant For all \c a we require that
  /// \code equal(plus(add_inv(a), a), zero) \endcode
  /// and
  /// \code equal(plus(a, add_inv(a)), zero) \endcode
  /// Additionally this function must be consistent with the
  /// inherited eq::equal operator. That is, for all \c a and \c b
  /// if \code equal(a, b) \endcode then
  /// \code equal(add_inv(a), add_inv(b)) \endcode
  /// This ensures that addititive inverses are
  /// not representation dependent.
  virtual T add_inv(const T& a) const = 0;

  /// The minus operation is equivalent to addition where the
  /// second argument is inverted.
  /// \param a The left operand which is added to the inverse of
  /// the right operand.
  /// \param b The right operand which will be inverted.
  /// \return \code plus(a, add_inv(b)) \endcode
  T minus(const T& a, const T& b) const;
};

template<class T> T group<T>::minus(const T& a, const T& b) const {
  return this->plus(a, this->add_inv(b));
}
} // namespace math
} // namespace dynsolver
#endif
