#ifndef DYNSOLVER_MATH_UTIL_H_
#define DYNSOLVER_MATH_UTIL_H_

#include "math/vector.h"
#include "math/matrix.h"
#include "math/square_matrix.h"

#include <cassert>
#include <vector>
#include <iostream>

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
  augmented.rref();
  augmented.split_vertically(matrix.get_cols(), &left, &right);
  if(!left.is_identity()) return false;
  vector result(matrix.size());
  for(int i = 0; i != right.get_rows(); ++i) {
    result[i] = right[i][0];
  }
  *solution = result;
  return true;
}

// Performs one round of euler's method. The initial vector indicates the
// the initial point and is of size m.
// The vectorField is a size n vector of functions which take as input
// a double array of size m. The indices variable associates to each function,
// in vectorField an index in the initial vector indicating which variable that
// particular function modifies. diffVar is the index in initial corresponding
// to the variable of differentiation. Inc is the increment to be used.
template<class FUNCTION>
void euler(vector& initial, const std::vector<FUNCTION>& vectorField,
	   const std::vector<size_t>& indices, size_t diffVar, double inc) {
  assert(inc != 0);
  int dimension = vectorField.size();
  
  vector scratch(initial);
  for(int i = 0; i != dimension; ++i) {
    initial[indices[i]] += inc * vectorField[i](scratch.data());
  }
  initial[diffVar] += inc;
}

// Performs one round of newton's method iteration to solve the system of n
// equations in n variables. Initial is a math::vector specifying what initial
// point is used as the start of newton's method. Functions is a std::vector of
// n functions which each function takes in a double* pointing to an array of
// variables and returns a double. Setting each function to 0 specifies a
// system to solve. Jacobian is a std::vector of n^2 functions arranged in row
// major order that specify the jacobian matrix function of the provided
// system.
// Indices indicates exactly which of the values in the initial vector
// correspond to the functions provided. In particular, the first function,
// in functions corresponds to the entry initial[indices[0]]. The second
// corresponds to initial[indices[1]]. All indices not specified in the
// provided vector are substituted as is into the function inputs.
// 
// Returns true if the iteration was successuful, and updates the
// initial vector. If the iteration failed, the initial vector
// remains unchanged.
template<class FUNCTION>
bool newton_iteration(vector& initial,
		      const std::vector<FUNCTION>& functions,
		      const std::vector<FUNCTION>& jacobianFunction,
		      const std::vector<size_t>& indices) {
  assert(indices.size() == functions.size());
  assert(functions.size() * functions.size() == jacobianFunction.size());
  int dimension = functions.size();
  
  // Evaluate the jacobian at the initial point.
  square_matrix jacobian(dimension);
  for(int r = 0; r != dimension; ++r) {
    for(int c = 0; c != dimension; ++c) {
      jacobian[r][c] = jacobianFunction[r*dimension + c](initial.data());
    }
  }

  // Compute b = -J^{-1}f(x) by solving Jb = -f(x)
  vector image(dimension);
  for(int i = 0; i != dimension; ++i) {
    image[i] = -1.0 * functions[i](initial.data());
  }
  vector increment;
  if(!solve_linear(jacobian, image, &increment)) return false;
  
  // Compute the result as initial + b. Note that we only update the values of
  // initial corresponding with dependent variables.
  for(int i = 0; i != indices.size(); ++i) {
    initial[indices[i]] += increment[i];
  }
  return true;
}

// Continually performs newtons iterations until either the max number of
// iterations has been reached, or the orbit has not remained within a
// small ball for a a certain number of iterations.
// If newtons method is successuful, the function returns true and updates,
// initial with the resultant value.
template<class FUNCTION>
bool newton_method(vector& initial,
		   const std::vector<FUNCTION>& functions,
		   const std::vector<FUNCTION>& jacobian,
		   const std::vector<size_t>& indices) {
  static const int maxIterations(100);
  static const int holdIterations(5);
  static const double tolerance(1e-10);
  
  // Counts the number of consecutive iterations where the
  // initial condition moved less than tolerance distance under the
  // iteration.
  int holdCount = 0;
  int i = 0;
  vector previous(initial);
  vector next(initial);
  while(i != maxIterations && holdCount != holdIterations) {
    if(!newton_iteration(next, functions, jacobian, indices)) {
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
  initial = next;
  return true;
}
} // namespace math
} // namespace dynsolver
#endif
