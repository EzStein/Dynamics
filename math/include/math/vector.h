#ifndef MATH_VECTOR_H_
#define MATH_VECTOR_H_

#include "math/matrix.h"
 
namespace math {

// Represents a vector or a point in an n dimensional space.
// Like the matrix, class this class is mutable.
class vector : public matrix {
 public:
  // Constructs an empty vector.
  vector();
  
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

  // Computes and returns the orthogonal projection of this vector
  // onto other.
  vector projection(const vector& other) const;

  // Computes the standard distance between this vector and another vector.
  double distance(const vector&) const;

  // Returns the angle between the two vectors.
  static double angle(const vector&, const vector&);

  // returns the dot product between both vectors.
  static double dot(const vector&, const vector&);

  // More arithmatic overloads
  vector operator+(const vector&) const;
  vector operator-(const vector&) const;
  vector operator*(double) const;
  vector operator/(double) const;
};
} // namespace math

#endif
