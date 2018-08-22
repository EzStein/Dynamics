#ifndef MATH_POLYNOMIAL_H_
#define MATH_POLYNOMIAL_H_

#include <cstddef>
#include <vector>
#include <string>

namespace math {

struct division_ret;
class poly_root;
class complex;

// The polynomial class represents a real polynomial of one variable
// whose coefficients are doubles. Provides functions for solving this
// polynomial over the complex field. 
class polynomial {
private:
  // An array of coefficients. The first entry is the constant term,
  // second entry is the linear term, third is quadratic.
  // Invariant: The leading coefficient is the last element in the array.
  // That is, the last element in the array is nonzero unless this polynomial
  // is the zero polynomial in which case it is the only element in the array.
  double* coefficients;

  // The size of the coefficients array. The degree of the polynomial is
  // one less than the size.
  int size;

  // The tolerance used to determine whether two values are equal. If two value
  // are exactly kTolerance apart they are considered not equal.
  const static double kTolerance;
  
public:
  // Constructs a polynomial containing only the constant zero.
  polynomial();
  
  // Constructs this poly from a vector of coefficients.
  // The first entry is the constant term, the second entry is the
  // coefficient of the linear term, the third entry is the quadratic etc...
  // If an empty vector is passed in, the polynomial created is the zero
  // polynomial.
  polynomial(const std::vector<double>& coefficients);

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
  int get_degree() const;

  // Converts to a string for debugging purposes.
  std::string to_string() const;

  // Returns true if this is within some tolerance of the constant,
  // zero polynomial.
  bool is_zero() const;

  // Evaluates the polynomial at the given value.
  double evaluate(double) const;

  // Evaluates the polynomial at a complex value.
  complex evaluate(const complex&) const;

  // Returns a polynomial representing the derivative of this polynomial.
  polynomial derivative() const;

  // Divides this polynomial by the divisor. Requires that the divisor is
  // non zero.
  division_ret divide(const polynomial& divisor) const;

  // Evaluates the polynomial and it's n derivatives at the value x.
  // Returns a vector of size n + 1.
  // The kth derivative is returned in the std::vector at index k. In particular
  // the evaluation is returned in the 0th index.
  //  std::vector<double> evaluate(size_t n, double x) const;
  //  std::vector<complex> evaluate(size_t n, complex x) const;

  // Finds all the roots of this polynomial returning a vector of these roots.
  // The multiplicities of each root in the vector will add up to get_degree().
  std::vector<poly_root> find_roots() const;

private:
  // Constructs this polynomial from a vector as in the constructor.
  void construct(const std::vector<double>&);
};

struct division_ret {
    polynomial quotient;
    polynomial remainder;
};

} // namespace math

#endif
