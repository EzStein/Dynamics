#ifndef DYNSOLVER_MATH_VECTOR_H_
#define DYNSOLVER_MATH_VECTOR_H_

#include "math/matrix.h"

namespace dynsolver {
namespace math {

// Represents a vector or a point in an n dimensional space.
// Like the matrix, class this class is mutable.
class vector : public matrix {
 public:
  // Constructs an uninitialized vector of the given size.
  vector(int size);

  // Constructs a vector where each entry has value, val.
  vector(int size, double val);

  // Returns a reference to the value at the given index.
  // Requires that index is nonnegative and less than size().
  double& operator[](int index);
  const double& operator[](int index) const;

  // Returns the number of entries in this vector.
  int size() const;
};
// More arithmatic overloads
vector operator+(vector, const vector&);
vector operator-(vector, const vector&);
vector operator*(matrix, const vector&);
vector operator*(double, vector);
vector operator*(vector, double);
vector operator/(vector, double);

// Returns the angle between the two vectors.
double angle(const vector&, const vector&);

// returns the dot product between both vectors.
double dot(const vector&, const vector&);
} // namespace math
} // namespace dynsolver
#endif
