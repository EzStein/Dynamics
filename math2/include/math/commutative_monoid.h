///
/// \file commutative_monoid.h
/// \author Ezra Stein
/// Interface class for a commutative monoid structure.
#ifndef DYNSOLVER_MATH_COMMUTATIVE_MONOID_H_
#define DYNSOLVER_MATH_COMMUTATIVE_MONOID_H_
namespace dynsolver {
namespace math {

/// A \c commutative_monoid is a \link monoid \endlink where the
/// magma::plus operation is commutative.
/// \invariant For all \c a and \c b,
/// \code equal(plus(a, b), plus(b, a)) \endcode
/// \tparam T The type on which this monoid operates.
template<class T> class commutative_monoid : public monoid<T> { };
} // namespace math
} // namespace dynsolver
#endif
