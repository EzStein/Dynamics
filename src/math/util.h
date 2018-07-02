
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
// and returns the result solution as a parameter. If the solution does not
// exist or is not unique (if mat is not invertible) then the function returns
// false and does not alter solution. Requires that matrix.size(), vector.size()
// are the same.
bool solve_linear(const square_matrix& matrix,
		  const vector& vec, vector* solution) {
  assert(vec.size() == matrix.size());
  
  math::matrix augmented = matrix::adjoin_by_row(matrix, vec);
  math::matrix left, right;
  augmented.rref().split_vertically(matrix.get_cols(), &left, &right);
  if(!left.is_identity()) return false;
  vector result(matrix.size());
  for(int i = 0; i != right.get_rows(); ++i) {
    result[i] = right[i][0];
  }
  *solution = result;
  return true;
}

// Performs one iteration of euler's (or similar) method. As a convention, the
// initial math::vector is an n + 1 sized vector where n > 0.
// The first entry is the variable of differentiation
// that is, the time variable. The rest of the components are position
// variables. vectorField is a std::vector of size n of functions that describe
// the derivative of each position variable with respect to the time variable.
// Each function takes in an array of (n + 1) doubles specifying the time and
// position variables in the same order that they appear in the initial vector.
// inc is any nonzero double specifying the granularity of the iteration.
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

// Performs one round of newton's method iteration to solve the system of n
// equations in n variables. initial is a math::vector specifying what initial
// point to used as the start of newton's method. functions is a std::vector of
// n functions which each function takes in a double* pointing to an array of n
// variables and returns a double. Setting each function to 0 specifies a
// system to solve. Jacobian is a std::vector of n^2 functions arranged in row
// major order that specify  the jacobian matrix function of the provided
// system.
// Returns true if the iteration was successuful, and fills the result vector.
//
// Currently we must append a 0 input as the t component whenever we call a
// function. This is not a permanent fix.
template<class FUNCTION>
bool newton_iteration(const vector& initial,
		      const std::vector<FUNCTION>& functions,
		      const std::vector<FUNCTION>& jacobianFunction,
		      vector* result) {
  assert(initial.size() == functions.size());
  assert(functions.size() * functions.size() == jacobianFunction.size());
  int dimension = functions.size();

  std::vector<double> scratch;
  scratch.push_back(0);
  for(int i = 0; i != dimension; ++i) {
    scratch.push_back(initial[i]);
  }
  // Evaluate the jacobian at the initial point.
  square_matrix jacobian(dimension);
  for(int r = 0; r != dimension; ++r) {
    for(int c = 0; c != dimension; ++c) {
      jacobian[r][c] = jacobianFunction[r*dimension + c](scratch.data());
    }
  }
  vector image(dimension);
  for(int i = 0; i != dimension; ++i) {
    image[i] = -1.0 * functions[i](scratch.data());
  }
  vector increment;
  if(!solve_linear(jacobian, image, &increment)) return false;
  *result = initial + increment;
  return true;
}

// Continually performs newtons iterations until either the max number of
// iterations has been reached, or the orbit has not remained within a
// small ball for a a certain number of iterations
template<class FUNCTION>
bool newton_method(const vector& initial,
		      const std::vector<FUNCTION>& functions,
		      const std::vector<FUNCTION>& jacobian,
		      vector* result) {
  const int maxIterations(100);
  const int holdIterations(5);
  const double tolerance(0.0001);
  
  // Counts the number of consecutive iterations where the
  // initial condition moved less than tolerance distance under the
  // iteration.
  int holdCount = 0;
  int i = 0;
  vector previous(initial);
  vector next;
  while(i != maxIterations && holdCount != holdIterations) {
    if(!newton_iteration(previous, functions, jacobian, &next)) {
      return false;
    }
    if(previous.distance(next) < tolerance) {
      ++holdCount;
    } else {
      holdCount = 0;
    }
    ++i;
    previous = next;
  }
  if(i == maxIterations) return false;
  *result = next;
  return true;
}

} // namespace math
} // namespace dynsolver
#endif
