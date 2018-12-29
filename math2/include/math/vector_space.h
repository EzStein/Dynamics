#ifndef DYNSOLVER_MATH_VECTOR_SPACE_H_
#define DYNSOLVER_MATH_VECTOR_SPACE_H_
#include "math/module.h"

namespace dynsolver {
namespace math {

// A vector space is a module in which the scalars are a field.
template<class S, class V> class vector_space : public module<S, T> {
 public:
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
  
  // Returns the field of scalars.
  virtual field<S> scalar_op() const override = 0;
};
}
}
#endif
