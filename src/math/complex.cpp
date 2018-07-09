#include "math/complex.h"

namespace dynsolver {
namespace math {

complex::complex(double real, double imag) : vector2d(real, imag) { }

double complex::real() const {
  return x();
}

double complex::imaginary() const {
  return y();
}

double& complex::real() {
  return x();
}

double& complex::imaginary() {
  return y();
}

void complex::operator*=(const complex& other) {
  double newReal = real() * other.real() - imaginary() * other.imaginary();
  double newImag =  real() * other.imaginary() + imaginary() * other.real();
  real() = newReal;
  imaginary() = newImag;
}

complex complex::operator*(const complex& other) const {
  complex num(*this);
  num *= other;
  return num;
}

complex complex::operator+(const complex& other) const {
  complex num(*this);
  num += other;
  return num;
}

complex complex::operator-(const complex& other) const {
  complex num(*this);
  num -= other;
  return num;
}

} // namespace math
} // namespace dynsolver
