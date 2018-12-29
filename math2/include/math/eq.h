#ifndef DYNSOLVER_MATH_EQ_H_
#define DYNSOLVER_MATH_EQ_H_
namespace dynsolver {
namespace math {

// Implicitly, all sets must be endowed with a notion of
// element equality.
//
// This class is an interface for implementing an equality
// measure on a set of type T.
template<class T> class eq {
 public:
  // Returns true if the two elements are equal. It is required that
  // for any a, b and c,
  // equal(a, b) == equal(b, a) and equal(a, a) == true.
  // and if equal(a, b) and equal(b, c), then equal(a, c)
  virtual bool equal(const T&, const T&) const = 0;
};
} // namespace math
} // namespace dynsolver   
#endif
