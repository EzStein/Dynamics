#include "math/polynomial.h"

#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <cassert>

#include "math/complex.h"
#include "math/poly_root.h"

namespace dynsolver {
namespace math {

const double polynomial::kTolerance = 1e-50;

polynomial::polynomial() : polynomial(0.0) { }

polynomial::polynomial(double constant) : coefficients(nullptr), size(1) {
  coefficients = new double[size];
  coefficients[0] = constant;
}

polynomial::polynomial(const std::vector<double>& other) {
  construct(other);
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
  int newSize(get_degree() + other.get_degree() + 1);
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
  std::vector<double> newCoefficients;
  if(other.size > size) {
    for(int i = 0; i != size; ++i) {
      newCoefficients.push_back(coefficients[i] + other.coefficients[i]);
    }
    for(int i = size; i != other.size; ++i) {
      newCoefficients.push_back(other.coefficients[i]);
    }
  } else {
    for(int i = 0; i != other.size; ++i) {
      newCoefficients.push_back(coefficients[i] + other.coefficients[i]);
    }
    for(int i = other.size; i != size; ++i) {
      newCoefficients.push_back(coefficients[i]);
    }
  }
  delete[] coefficients;
  construct(newCoefficients);
}

void polynomial::construct(const std::vector<double>& other) {
  int lead = other.size() - 1;
  while(lead >= 0) {
    if(std::abs(other[lead]) >= kTolerance) {
      break;
    }
    --lead;
  }
  if(lead < 0) {
    size = 1;
    coefficients = new double[size];
    coefficients[0] = 0.0;
  } else {
    size = lead + 1;
    coefficients = new double[size];
    std::copy(other.data(), other.data() + size, coefficients);
  }
}

void polynomial::operator-=(const polynomial& other) {
  std::vector<double> newCoefficients;
  if(other.size > size) {
    for(int i = 0; i != size; ++i) {
      newCoefficients.push_back(coefficients[i] - other.coefficients[i]);
    }
    for(int i = size; i != other.size; ++i) {
      newCoefficients.push_back(-1 * other.coefficients[i]);
    }
  } else {
    for(int i = 0; i != other.size; ++i) {
      newCoefficients.push_back(coefficients[i] - other.coefficients[i]);
    }
    for(int i = other.size; i != size; ++i) {
      newCoefficients.push_back(coefficients[i]);
    }
  }
  delete[] coefficients;
  construct(newCoefficients);
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

int polynomial::get_degree() const {
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
  return std::abs(coefficients[0]) < kTolerance && size == 1;
}

double polynomial::evaluate(double x) const {
  double acc = coefficients[size - 1];
  for(int i = 1; i != size; ++i) {
    acc = coefficients[size - i - 1] + acc * x;
  }
  return acc;
}

complex polynomial::evaluate(const complex& x) const {
  complex acc(coefficients[size - 1], 0);
  for(int i = 1; i != size; ++i) {
    acc = complex(coefficients[size - i - 1], 0) + acc * x;
  }
  return acc;
}

polynomial polynomial::derivative() const {
  std::vector<double> derivative;
  for(int i = 1; i != size; ++i) {
    derivative.push_back(coefficients[i] * i);
  }
  if(derivative.size() == 0) {
    derivative.push_back(0.0);
  }
  return polynomial(derivative);
}

// We shall use synthetic division of monomials.
division_ret polynomial::divide(const polynomial& divisor) const {
  assert(!divisor.is_zero());
  // The index of the divisor's leading coefficient.
  int leadCoef = divisor.size - 1;
  
  // We invert the divisor and make it a monomial.
  polynomial monDiv = divisor*polynomial(-1.0 / divisor.coefficients[leadCoef]);

  // Deflation represents the synthetic division deflation. The last
  // leadCoef - 1 elements are coefficients of the remainder
  // in reverse and the remaining first elements are coefficients of the
  // quotient in reverse.
  std::vector<double> deflation(size);
  for(int i = 0; i != size; ++i) {
    double acc(coefficients[size - i - 1]);
    for(int j = 0; j != leadCoef; ++j) {
      int deflationIndex = i - leadCoef + j;
      if(deflationIndex >= 0 && i + j < size) {
	acc += deflation[deflationIndex] * monDiv.coefficients[j];
      }
    }
    deflation[i] = acc;
  }
  
  std::vector<double> quotient;
  // Copy and reverse the quotient
  for(int i = size - leadCoef - 1; i >= 0; --i) {
    quotient.push_back(deflation[i]);
  }

  std::vector<double> remainder;
  for(int i = size - 1; i >= size - leadCoef && i >= 0; --i) {

    remainder.push_back(deflation[i]);
  }
  polynomial q(polynomial(quotient)
	       * polynomial(1.0 / divisor.coefficients[leadCoef]));
  polynomial r(remainder);
  return division_ret{q, r};
}

// We shall use Lagurre's method with polynomial deflation followed by
// polishing with newton's method.
std::vector<poly_root> polynomial::find_roots() const {
  polynomial deflated(*this);
  polynomial deflatedDeriv1(deflated.derivative());
  polynomial deflatedDeriv2(deflatedDeriv1.derivative());
  assert(false);
}
} // namespace math
} // namespace dynsolver
