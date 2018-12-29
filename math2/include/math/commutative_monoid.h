#ifndef DYNSOLVER_MATH_COMMUTATIVE_MONOID_H_
#define DYNSOLVER_MATH_COMMUTATIVE_MONOID_H_
namespace dynsolver {
namespace math {

// A commutative monoid is a monoid in which the addition operation is
// commutative.
// Requires that for all a and b
// equal(plus(a, b), plus(b, a))
// This class provides no additional functionality over a monoid,
// but it allows a client to enforce commutativity using the type checker.
template<class T> class commutative_monoid : public monoid<T> { };
} // namespace math
} // namespace dynsolver
#endif
