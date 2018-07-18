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

  double norm_squared() const;

  // Returns true if the imaginary part is near zero.
  bool is_real() const;

  std::string to_string() const;

  // Converts this number to its principle square root. If a principle square
  // root does not exist (when z is a non-positive real number) then the
  // root on the nonnegative imaginary axis is chosen.
  void sqrt();

  // Conjugates this complex number.
  void conjugate();

  // Inverts this complex number. Requires that the number is not 0 + 0i.
  void invert();

  // Multiplies this complex number with the other mutating this.
  void operator*=(const complex& other);
  void operator/=(const complex& other);

  complex operator*(const complex& other) const;
  complex operator+(const complex& other) const;
  complex operator-(const complex& other) const;
  complex operator/(const complex& other) const;
}; 
} // namespace math
} // namespace dynsolver 
#endif
