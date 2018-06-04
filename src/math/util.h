#ifndef DYNSOLVER_MATH_UTIL_H_
#define DYNSOLVER_MATH_UTIL_H_

#include "math/vector.h"
#include "math/matrix.h"
#include "math/square_matrix.h"

#include <cassert>
#include <vector>

namespace dynsolver {
namespace math {


// Solves the system of linear equations, matrix * solution = vec
// and returns the result solution as a parameter. If the solution does not exist or is not unique
// (if mat is not invertible) then the function returns false and does not alter solution.
// Requires that matrix.size(), vector.size() and solution.size() are all the same.
bool solve_linear(const square_matrix& matrix, const vector& vec, vector& solution) {
  assert(vec.size() == matrix.size());
  assert(matrix.size() == solution.size());
  math::matrix augmented = matrix::adjoin_by_row(matrix, vec);
  augmented.rref();
  augmented.set_ones();
  augmented.set_zeros();
  
  for(int r = 0; r != augmented.get_rows(); ++r) {
    for(int c = 0; c != augmented.get_cols() - 1; ++c) {
      if(r == c) {
        if(augmented[r][c] != 1)
          return false;
      } else {
        if(augmented[r][c] != 0)
          return false;
      }
    }
  }
  for(int i = 0; i != augmented.get_rows(); ++i) {
    solution[i] = augmented[i][augmented.get_cols() - 1];
  }
  return true;
}

// Performs one iteration of euler's (or similar) method. As a convention, the initial
// math::vector is an n + 1 sized vector where n > 0.
// The first entry is the variable of differentiation
// that is, the time variable. The rest of the components are position variables.
// vectorField is a std::vector of size n of functions that describe the derivative
// of each position variable with respect to the time variable. Each function takes in
// an array of (n + 1) doubles specifying the time and position variables in the same order
// that they appear in the initial vector. inc is any nonzero double specifying the
// granularity of the iteration.
// scratch is a pointer to an array of memory large enough to fit n doubles.
//
// The initial vector is upaded with the computed value.
template<class FUNCTION>
void euler(vector& initial, const std::vector<FUNCTION>& vectorField,
           double inc) {
  assert(inc != 0);
  assert(initial.size() >= 2);
  assert(vectorField.size() + 1 == initial.size());
  int dimension = vectorField.size();
  vector scratch(dimension + 1);
  scratch[0] = inc;
  for(int i = 0; i != dimension; ++i) {
    scratch[i + 1] = inc * vectorField[i](initial.data());
  }
  initial += scratch;
}

// Performs the same operation as the above function but substitures in the parameters.
template<class FUNCTION>
void euler(vector& initial, const std::vector<FUNCTION>& vectorField,
           const vector& parameters,
           double inc) {
  assert(inc != 0);
  assert(initial.size() >= 2);
  assert(vectorField.size() + 1 == initial.size());
  int dimension = vectorField.size();
  vector params(initial.size() + parameters.size());
  
  for(int i = 0; i != initial.size(); ++i)
    params[i] = initial[i];
  
  for(int i = 0; i != parameters.size(); ++i)
    params[i + initial.size()] = parameters[i];
  
  vector scratch(dimension + 1);
  scratch[0] = inc;
  for(int i = 0; i != dimension; ++i) {
    scratch[i + 1] = inc * vectorField[i](params.data());
  }
  initial += scratch;
}

// Performs one round of newton's method iteration to solve the system of n equations
// in n variables. initial is a math::vector specifying what initial point to used
// as the start of newton's method. functions is a std::vector of n functions which
// where each function takes in a double* pointing to an array of n variables and
// returns a double. Setting each function to 0 specifies a system to solve.
// Jacobian is a std::vector of n^2 functions arranged in row major order that specify
// the jacobian matrix function of the provided system.
//
// Returns true and updates initial if the jacobian is nonsingular at the initial point.
// Otherwise returns false and does not update initial.
template<class FUNCTION>
bool newton(vector& initial, const std::vector<FUNCTION>& functions,
            const std::vector<FUNCTION>& jacobianFunction) {
  assert(initial.size() == functions.size());
  assert(functions.size() * functions.size() == jacobianFunction.size());
  int dimension = functions.size();
  square_matrix jacobian(dimension);
  for(int r = 0; r != dimension; ++r) {
    for(int c = 0; c != dimension; ++c) {
      jacobian[r][c] = jacobianFunction[r*dimension + c](initial.data());
    }
  }
  vector image(dimension);
  for(int i = 0; i != dimension; ++i) {
    image[i] = -1.0 * functions[i](initial.data());
  }
  vector increment(dimension);
  if(!solve_linear(jacobian, image, increment)) return false;
  initial += increment;
  return true;
}
} // namespace math
} // namespace dynsolver
#endif
