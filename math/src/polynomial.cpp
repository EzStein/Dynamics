#include "math/polynomial.h"

#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <cassert>
#include <iostream>

#include "math/complex.h"
#include "math/poly_root.h"

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
  const double tolerance = 1e-10;
  const int maxIterations = 1000;
  // This vector holds a list of potential roots that have yet to be polished.
  std::vector<complex> potentialRoots;

  // This is the polynomial for which we will find a root.
  polynomial deflated(*this);
  while(1) {
    // Our initial guess at the root.
    complex root(0, 0);

    // The Degree of the polynomial
    complex n(deflated.get_degree(), 0);
    polynomial deriv(deflated.derivative());
    polynomial deriv2(deriv.derivative());
    for(int i = 0; i != maxIterations; ++i) {
      complex eval(deflated.evaluate(root));
      if(eval.norm() < tolerance) break;
      complex evalDeriv(deriv.evaluate(root));
      complex evalDeriv2(deriv2.evaluate(root));
      complex g = evalDeriv / eval;
      complex gSquared = g * g;
      complex h = gSquared - evalDeriv2 / eval;
      complex sqrt = ((n - complex(1,0)) * (n * h - gSquared));
      sqrt.sqrt();
      complex denomMinus = g - sqrt;
      complex denomPlus = g + sqrt;
      complex change(denomMinus.norm() > denomPlus.norm() ?
		     n / denomMinus : n / denomPlus);
      root -= change;
      if(change.norm() < tolerance) break;
    }
    // We now have a potential root stored in roots. We first polish it before
    // using synthetic division to get a new polynomial for seraching
    // for the remaining roots.
    if(std::abs(root.imaginary()) < tolerance) {
      // Real root
      potentialRoots.push_back(root);
      polynomial divisor(std::vector<double>{-1 * root.real(), 1});
      deflated = deflated.divide(divisor).quotient;
    } else {
      // Complex conjugate Root we add it and its conjugate.
      potentialRoots.push_back(root);
      root.conjugate();
      potentialRoots.push_back(root);
      
      std::vector<double> coefficients;
      coefficients.push_back(root.norm_squared());
      coefficients.push_back(-2 * root.real());
      coefficients.push_back(1);
      polynomial divisor(coefficients);
      deflated = deflated.divide(divisor).quotient;
    }
    if(deflated.get_degree() == 0) break;
  }
  // We now polish the roots with newton's method against the
  // original polynomial.
  std::vector<poly_root> roots;
  polynomial deriv(derivative());
  for(std::vector<complex>::iterator root = potentialRoots.begin();
      root != potentialRoots.end(); ++root) {
    for(int i = 0; i != maxIterations; ++i) {
      complex eval(evaluate(*root));
      complex evalDeriv(deriv.evaluate(*root));
      if(evalDeriv.norm() < tolerance) break;
      complex change = eval / evalDeriv;
      *root = *root - change;
      if(change.norm() < tolerance) break;
    }
    // We now add root to the list of poly_roots, increasing the multiplicity,
    // if it is not distinct.
    bool distinct = true;
    for(std::vector<poly_root>::iterator iter = roots.begin();
	iter != roots.end(); ++iter) {
      if(iter->get_root().distance(*root) < 1e-5) {
	iter->set_multiplicity(iter->get_multiplicity() + 1);
	distinct = false;
	break;
      }
    }
    if(distinct) {
      roots.push_back(poly_root(*root, 1));
    }
  }
  return roots;
  /*  std::vector<poly_root> roots;
  for(std::vector<complex>::iterator root = potentialRoots.begin();
      root != potentialRoots.end(); ++root) {
    roots.push_back(poly_root(*root, 1));
  }
  return roots;*/
}
} // namespace math

