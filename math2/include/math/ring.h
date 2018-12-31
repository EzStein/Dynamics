///
/// \file ring.h
/// \author Ezra Stein
/// Interface class for ring structure.
#ifndef DYNSOLVER_MATH_RING_H_
#define DYNSOLVER_MATH_RING_H_
#include "math/eq.h"
#include "math/monoid.h"
#include "math/abelian_group.h"

namespace dynsolver {
namespace math {

/// A \c ring is a set with an two operators.
/// One operator forms an abelian group which we call addition
/// and the other forms a monoid which we call multiplication.
/// The multiplication operator distributes over the
/// adition operator.
/// \invariant The equal operators of the
/// \link mult_op() multiplicative monoid \endlink
/// and \link add_op() additive group \endlink
/// must be consistent. That is, for all \c a and \c b,
/// \code mult_op().equal(a, b) \endcode
/// if and only if \code add_op().equal(a, b) \endcode
/// Additionally, the multiplication operation must distribute
/// over addition. For all \c a, \c b, and \c c,
/// \code equal(mult_op().plus(add_op().plus(a, b), c),
/// add_op().plus(mult_op().plus(a, c), mult_op().plus(b, c))) \endcode
/// and
/// \code equal(mult_op().plus(c, add_op().plus(a, b)),
/// add_op().plus(mult_op().plus(c, a), mult_op().plus(c, b))) \endcode
/// \tparam T The type of the elements on which this ring operates.
template<class T> class ring : public eq<T> {
 public:
  /// Retrieves the multiplication monoid.
  /// \return The multiplication monoid. The same or functionally,
  /// equivalent object must be returned each time.
  /// \invariant See \link ring \endlink
  virtual monoid<T> mult_op() const = 0;

  /// Retrieves the addition group.
  /// \return The addition monoid. The same or functionally,
  /// equivalent object must be returned each time.
  /// \invariant See \link ring \endlink
  virtual abelian_group<T> add_op() const = 0;

  /// The eq::equal operator for this ring is equivalent to the
  /// eq::equal operator of the
  /// \link mult_op() multiplicative monoid \endlink
  /// and \link add_op() additive group \endlink.
  /// \param a The left side of the equality check.
  /// \param b The right ide of the equality check.
  /// \return \c true if \p a equals \p b according to either the
  /// \link mult_op() multiplicative monoid's \endlink
  /// or \link add_op() additive group's \endlink
  /// implementation of eq::equal.
  /// \invariant See \link ring \endlink
  virtual bool equal(const T& a, const T& b) const override final;

  /// Alias for the \link mult_op() multiplicative monoid's \endlink
  /// binary operator.
  /// \param a The left operand for multiplication.
  /// \param b The right operand for multiplication.
  /// \return The product of \c a and \c b
  /// \code mult_op().plus(a, b) \endcode
  /// \sa magma::plus
  T times(const T& a, const T& b) const;

  /// Alias for the \link add_op() additive group's \endlink
  /// binary operator.
  /// \param a The left operand for addition.
  /// \param b The right operand for addition.
  /// \return The sum of \c a and \c b,
  /// \code add_op().plus(a, b) \endcode
  /// \sa magma::plus
  T plus(const T& a, const T& b) const;

  /// Alias for the \link add_op() additive group's \endlink
  /// inverse operation.
  /// \param a The element on which to find the additive inverse.
  /// \return The additive inverse of \c a,
  /// \code add_op().add_inv(a) \endcode
  /// \sa group::add_inv
  T add_inv(const T& a) const;

  /// Alias for the \link add_op() additive group's \endlink
  /// identity element
  /// \return The additive identity,
  /// \code add_op().zero() \endcode
  /// \sa monoid::zero
  T zero() const;

  /// Alias for the \link mult_op() multiplicative monoid's \endlink
  /// identity element
  /// \return The multiplicative identity,
  /// \code mult_op().zero() \endcode
  /// \sa monoid::zero
  T one() const;
};

template<class T> bool ring<T>::equal(const T& a, const T& b) const {
  return this->mult_op().equal(a, b);
}

template<class T> T ring<T>::times(const T& a, const T& b) const {
  return this->mult_op().plus(a, b);
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
