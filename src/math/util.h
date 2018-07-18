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
		  const vector& vec, vector* solution);


// Updates the vector accordingly.
template<class FUNCTION>
void euler(const std::vector<FUNCTION>& vectorField,
	   double inc,
	   const std::vector<int>& indices,
	   int varDiff,
	   math::vector& input) {
  assert(!vectorField.empty());
  assert(vectorField.size() == indices.size());
  int dimension = vectorField.size();

  std::vector<double> increment(dimension);
  for(int i = 0; i != dimension; ++i) {
    increment[i] = inc * vectorField[i](input.data());
  }
  for(int i = 0; i != dimension; ++i) {
    input[indices[i]] += increment[i];
  }
  input[varDiff] += inc;
}

// Continually performs newton iterations until either the max number of
// iterations has been reached, or the orbit has not remained within a
// small ball for a a certain number of iterations.
// If newtons method is successuful, the function returns true and updates,
// initial with the resultant value.
template<class FUNCTION>
bool newton_method(const std::vector<FUNCTION>& functions,
		   const std::vector<FUNCTION>& jacobian,
		   const std::vector<int>& varIndices,
		   math::vector& input) {
  static const int maxIterations(100);
  static const int holdIterations(5);
  static const double tolerance(1e-10);
  
  // Counts the number of consecutive iterations where the
  // initial condition moved less than tolerance distance under the
  // iteration.
  int holdCount = 0;
  int i = 0;
  vector previous(input);
  vector next(input);
  while(i != maxIterations && holdCount != holdIterations) {
    if(!newton_iteration(functions, jacobian, varIndices, next)) {
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
  input = next;
  return true;
}


// Performs one round of newton's method, returning the new vector
// to the refinement. The returned vector's variables correspond
// with those contained in functions. jacobianFunction represents
// the row major jacobian of the vector of functions. input is the
// array used as input for all the functions.
// indices associates each variable in functions to the corresponding variable
// in input. Those variables in input are updated to reflect
// the change.
template<class FUNCTION>
bool newton_iteration(const std::vector<FUNCTION>& functions,
		      const std::vector<FUNCTION>& jacobianFunction,
		      const std::vector<int>& varIndex,
		      math::vector& input) {
  assert(!functions.empty());
  assert(functions.size() * functions.size() == jacobianFunction.size());
  assert(functions.size() == varIndex.size());
  
  int dimension = functions.size();
  
  // Evaluate the jacobian at the initial point.
  square_matrix jacobian(dimension);
  for(int r = 0; r != dimension; ++r) {
    for(int c = 0; c != dimension; ++c) {
      jacobian[r][c] = jacobianFunction[r*dimension + c](input.data());
    }
  }

  // Compute b = -J^{-1}f(x) by solving Jb = -f(x)
  vector image(dimension);
  for(int i = 0; i != dimension; ++i) {
    image[i] = -1.0 * functions[i](input.data());
  }
  
  vector increment;
  if(!solve_linear(jacobian, image, &increment)) return false;
  
  // Compute the result as initial + b. Note that we only update the values of
  // initial corresponding with dependent variables.
  for(int i = 0; i != dimension; ++i) {
    input[varIndex[i]] += increment[i];
  }
  return true;
}
} // namespace math
} // namespace dynsolver
#endif
