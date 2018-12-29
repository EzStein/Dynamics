#ifndef DYNSOLVER_MATH_MODULE_H_
#define DYNSOLVER_MATH_MODULE_H_
#include "math/abelian_group.h"
#include "math/ring.h"

namespace dynsolver {
namespace math {

// A module is an abelian group of vectors with a ring
// of scalars along with a binary operator between
// scalars and vectors which returns vectors.
template<class S, class V> class module {
 public:
  // Properties for all vectors v, w and scalars a, b:
  //
  // vector_op().equal(
  // times(a, vector_op().plus(v, w)),
  // vector_op().plus(times(a, v), times(a, w)))
  //
  // vector_op().equal(
  // times(scalar_op().plus(a, b), v)
  // vector_op().plus(times(a, v), times(b, v)))
  //
  // vector_op().equal(
  // times(scalar_op().times(a, b), v),
  // times(a, times(b, v)))
  //
  // vector_op().equal(
  // times(scalar_op().one(), v), v)
  
  // Returns the ring of scalars.
  virtual ring<S> scalar_op() const = 0;

  // Returns the group of vectors.
  virtual abelian_group<V> vector_op() const = 0;

  // Returns the scalar multuplication of the provided
  // scalar and vector.
  virtual V times(const S&, const V&) const = 0;
};
} // namespace math
} // namespace dynsolver
#endif
