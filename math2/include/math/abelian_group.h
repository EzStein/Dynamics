#ifndef DYNSOLVER_MATH_ABELIAN_GROUP_H_
#define DYNSOLVER_MATH_ABELIAN_GROUP_H_
#include "math/group.h"

namespace dynsolver {
namespace math {

// In algebra, an abelian group is a commutative group.
// This class provides no additional functionality over
// the group class but it allows for clients to force commutativity
// of the plus operator with the type checker.
//
// We require for all a and b,
// equal(plus(a, b), plus(b, a))
template<class T> abelian_group : public group<T> { };
} // namespace math
} // namespace dynsolver
#endif
