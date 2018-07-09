#include "math/polynomial.h"

#include <vector>
#include <string>
#include <sstream>
#include <cstring>

namespace dynsolver {
namespace math {

polynomial::polynomial() : polynomial(0.0) { }

polynomial::polynomial(double constant) : coefficients(nullptr), size(1) {
  coefficients = new double[size];
  coefficients[0] = constant;
}

polynomial::polynomial(const std::vector<double>& other)
  : coefficients(nullptr), size(other.size()) {
  coefficients = new double[size];
  std::copy(other.begin(), other.end(), coefficients);
}

polynomial::polynomial(const double* other, size_t otherSize)
  : coefficients(nullptr), size(otherSize) {
  coefficients = new double[size];
  std::copy(other, other + size, coefficients);
}

polynomial::~polynomial() {
  delete[] coefficients;
}

polynomial::polynomial(const polynomial& other)
  : coefficients(nullptr), size(other.size) {
  coefficients = new double[size];
  std::copy(other.coefficients, other.coefficients + size, coefficients);
}

polynomial::polynomial(polynomial&& other)
  : coefficients(other.coefficients), size(other.size) {
  other.coefficients = nullptr;
}

polynomial& polynomial::operator=(const polynomial& other) {
  if(this == &other) return *this;
  delete[] coefficients;
  size = other.size;
  coefficients = new double[size];
  std::copy(other.coefficients, other.coefficients + size, coefficients);
  return *this;
}

polynomial& polynomial::operator=(polynomial&& other) {
  if(this == &other) return *this;
  delete[] coefficients;
  coefficients = other.coefficients;
  size = other.size;
  other.coefficients = nullptr;
  return *this;
}

// This is implemented in the naive O(n^2) algorithm. There exists a
// much more complicated O(n log(n)) algorithm using FFT.
void polynomial::operator*=(const polynomial& other) {
  size_t newSize(get_degree() + other.get_degree() + 1);
  double* newCoefficients = new double[newSize];
  for(int i = 0; i != newSize; ++i) {
    newCoefficients[i] = 0;
    for(int j = 0; j != i + 1; ++j) {
      double lhs = j < size ? coefficients[j] : 0;
      double rhs = i - j < other.size ? other.coefficients[i - j] : 0;
      newCoefficients[i] += lhs * rhs;
    }
  }
  delete[] coefficients;
  coefficients = newCoefficients;
  size = newSize;
}

void polynomial::operator+=(const polynomial& other) {
  size_t newSize = std::max(size, other.size);
  double* newCoefficients = new double[newSize];
  if(other.size > size) {
    for(int i = 0; i != size; ++i) {
      newCoefficients[i] = coefficients[i] + other.coefficients[i];
    }
    for(int i = size; i != other.size; ++i) {
      newCoefficients[i] = other.coefficients[i];
    }
  } else {
    for(int i = 0; i != other.size; ++i) {
      newCoefficients[i] = coefficients[i] + other.coefficients[i];
    }
    for(int i = other.size; i != size; ++i) {
      newCoefficients[i] = coefficients[i];
    }
  }
  delete[] coefficients;
  size = newSize;
  coefficients = newCoefficients;
}

void polynomial::operator-=(const polynomial& other) {
  size_t newSize = std::max(size, other.size);
  double* newCoefficients = new double[newSize];
  if(other.size > size) {
    for(int i = 0; i != size; ++i) {
      newCoefficients[i] = coefficients[i] - other.coefficients[i];
    }
    for(int i = size; i != other.size; ++i) {
      newCoefficients[i] = -1 * other.coefficients[i];
    }
  } else {
    for(int i = 0; i != other.size; ++i) {
      newCoefficients[i] = coefficients[i] - other.coefficients[i];
    }
    for(int i = other.size; i != size; ++i) {
      newCoefficients[i] = coefficients[i];
    }
  }
  delete[] coefficients;
  size = newSize;
  coefficients = newCoefficients;
}

polynomial polynomial::operator*(const polynomial& other) const {
  polynomial poly(*this);
  poly *= other;
  return poly;
}

polynomial polynomial::operator+(const polynomial& other) const {
  polynomial poly(*this);
  poly += other;
  return poly;
}

polynomial polynomial::operator-(const polynomial& other) const {
  polynomial poly(*this);
  poly -= other;
  return poly;
}

size_t polynomial::get_degree() const {
  return size - 1;
}

std::string polynomial::to_string() const {
  std::stringstream sstream;
  sstream << coefficients[0];
  for(int i = 1; i != size; ++i) {
    sstream << " + " << coefficients[i] << "x^" << i;
  }
  return sstream.str();
}

bool polynomial::is_zero() const {
  const double tolerance = 10e-15;
  for(int i = 0; i != size; ++i) {
    if(std::abs(coefficients[i]) >= tolerance) return false;
  }
  return true;
}
} // namespace math
} // namespace dynsolver
