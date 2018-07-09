#ifndef DYNSOLVER_MATH_COMPLEX_H_
#define DYNSOLVER_MATH_COMPLEX_H_

#include "math/vector2d.h"

namespace dynsolver {
namespace math {

// Represents a complex number as a 2d vector.
class complex : public vector2d {
public:
  // Constructs a complex number from real and imaginary components.
  complex(double real, double imag);

  // Gets the real component.
  double real() const;

  // Gets the imaginary component.
  double imaginary() const;

  double& real();
  double& imaginary();

  // Multiplies this complex number with the other mutating this.
  void operator*=(const complex& other);

  complex operator*(const complex& other) const;
  complex operator+(const complex& other) const;
  complex operator-(const complex& other) const;
}; 
} // namespace math
} // namespace dynsolver 
#endif
