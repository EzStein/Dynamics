#ifndef DYNSOLVER_MATH_SEMIGROUP_H_
#define DYNSOLVER_MATH_SEMIGROUP_H_
#include "math/magma.h"

namespace dynsolver {
namespace math {

// In algebra, a semigroup is a set with an associative binary
// operation. That is, it is an associative magma.
//
// This class does not offer any additional functionality over a
// magma but it allows for clients to enforce associativity using
// the type checker.
//
// We require for all a, b and c
// equal(plus(plus(a, b), c), plus(a, plus(b, c)))
template<class T> class semigroup : public magma<T> { };
} // namespace math
} // namespace dynsolver
#endif
