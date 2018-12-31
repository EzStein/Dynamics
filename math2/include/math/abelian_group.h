///
/// \file abelian_group.h
/// \author Ezra Stein
/// Interface class for a commutative group structure.
///
#ifndef DYNSOLVER_MATH_ABELIAN_GROUP_H_
#define DYNSOLVER_MATH_ABELIAN_GROUP_H_
#include "math/group.h"

namespace dynsolver {
namespace math {

/// An \c abelian_group is a \link group \endlink where the
/// magma::plus operation is commutative.
/// \invariant For all \c a and \c b,
/// \code equal(plus(a, b), plus(b, a)) \endcode
/// \tparam T The type on which this group operates.
template<class T> class abelian_group : public group<T> { };
} // namespace math
} // namespace dynsolver
#endif
