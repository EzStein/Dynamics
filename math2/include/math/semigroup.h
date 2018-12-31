///
/// \file semigroup.h
/// \author Ezra Stein
/// Interface class for associative binary operators.
///
#ifndef DYNSOLVER_MATH_SEMIGROUP_H_
#define DYNSOLVER_MATH_SEMIGROUP_H_
#include "math/magma.h"

namespace dynsolver {
namespace math {

/// A \c semigroup is a set with an associative binary
/// operator. That is, it is equivalent to a \link magma \endlink
/// with the additional requirement that the magma::plus
/// operator is associative.
/// \invariant
/// For all \c a, \c b, and \c c which have type \c T,
/// \code equal(plus(plus(a, b), c), plus(a, plus(b, c))) \endcode
template<class T> class semigroup : public magma<T> { };
} // namespace math
} // namespace dynsolver
#endif
