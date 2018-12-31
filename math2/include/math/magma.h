///
/// \file magma.h
/// \author Ezra Stein
/// Base interface class for binary operators.
///
#ifndef DYNSOLVER_MATH_MAGMA_H_
#define DYNSOLVER_MATH_MAGMA_H_
#include "math/eq.h"

namespace dynsolver {
namespace math {

/// A \c magma is a set endowed with a binary operator.
/// No additional restrictions on the binary operator other than
/// closure are implied by this interface. This interface is used
/// to separate binary operations from the objects they operate on
/// allowing one to supply custom binary operations to numerical
/// algorithms.
/// \tparam T The type of objects on which the magma operates.
template<class T> class magma : public eq<T> {
 public:
  /// The binary operator of this magma.
  /// \param a The left operand of the binary operator.
  /// \param b The right operand of the binary operator.
  /// \return The result of the binary operation applied to
  /// to \p a and \p b.
  /// \invariant The binary operator must be consistent with
  /// the inherited equality comparator. That is,
  /// for all \c a, \c b, and \c c, if
  /// \code equal(a, b) \endcode then
  /// \code equal(plus(a, c), plus(b, c)) \endcode
  /// and
  /// \code equal(plus(c, a), plus(c, b)) \endcode
  /// \sa eq::equal for a description of the equivalence relation.
  virtual T plus(const T& a, const T& b) const = 0;
};

} // namespace math
} // namespace dynsolver

#endif
