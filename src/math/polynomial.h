#ifndef DYNSOLVER_MATH_POLYNOMIAL_H_
#define DYNSOLVER_MATH_POLYNOMIAL_H_

#include <cstddef>
#include <vector>
#include <string>

namespace dynsolver {
namespace math {
// The polynomial class represents a real polynomial of one variable
// whose coefficients are doubles. Provides functions for solving this
// polynomial over the complex field. 
class polynomial {
private:
  // An array of coefficients. The first entry is the constant term,
  // second entry is the linear term, third is quadratic.
  double* coefficients;

  // The size of the coefficients array. The degree of the polynomial is
  // one less than the size.
  size_t size;
  
public:
  // Constructs a polynomial containing only the constant zero.
  polynomial();
  // Constructs this poly from a vector of coefficients.
  // The first entry is the constant term, the second entry is the
  // coefficient of the linear term, the third entry is the quadratic etc...
  polynomial(const std::vector<double>& coefficients);
  polynomial(const double* coefficients, size_t size);

  // Constructs a constant polynomial whose sole term is val.
  polynomial(double val);

  polynomial(const polynomial&);
  polynomial(polynomial&&);

  polynomial& operator=(const polynomial&);
  polynomial& operator=(polynomial&&);

  ~polynomial();

  // Arithmetic operations that mutate this polynomial.
  void operator*=(const polynomial&);
  void operator+=(const polynomial&);
  void operator-=(const polynomial&);

  // Arithmetic expressions which produce a new polynomial.
  polynomial operator*(const polynomial&) const;
  polynomial operator-(const polynomial&) const;
  polynomial operator+(const polynomial&) const;

  // Returns the degree of this polynomial. By convention, the constant
  // polynomial 0 is given degree 0 even though this is not mathematically
  // precise.
  size_t get_degree() const;

  // Converts to a string for debugging purposes.
  std::string to_string() const;

  // Returns true if this is within some tolerance of the constant,
  // zero polynomial.
  bool is_zero() const;
};
} // namespace math
} // namespace dynsolver


#endif
