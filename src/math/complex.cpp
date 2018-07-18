#include "math/complex.h"

#include <cmath>
#include <sstream>
#include <iostream>

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

bool complex::is_real() const {
  return std::abs(imaginary()) < kTolerance;
}

void complex::conjugate() {
  imaginary() *= -1;
}

double complex::norm_squared() const {
  return real() * real() + imaginary() * imaginary();
}

void complex::invert() {
  double normSquared = norm_squared();
  conjugate();
  real() /= normSquared;
  imaginary() /= normSquared;
}

void complex::sqrt() {
  const double tolerance = 1e-20;
  if(std::abs(imaginary()) < tolerance && real() < tolerance) {
    imaginary() = std::sqrt(std::abs(real()));
    real() = 0;
  } else {
    double modulus = norm();
    operator+=(complex(modulus, 0));
    operator*=(complex(std::sqrt(modulus) / norm(), 0));
  }
}

void complex::operator*=(const complex& other) {
  double newReal = real() * other.real() - imaginary() * other.imaginary();
  double newImag =  real() * other.imaginary() + imaginary() * other.real();
  real() = newReal;
  imaginary() = newImag;
}

void complex::operator/=(const complex& other) {
  complex inverse(other);
  inverse.invert();
  operator*=(inverse);
}

complex complex::operator/(const complex& other) const {
  complex num(*this);
  num /= other;
  return num;
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

std::string complex::to_string() const {
  std::stringstream sstream;
  sstream << real() << " + " << imaginary() << "i";
  return sstream.str();
}
} // namespace math
} // namespace dynsolver
