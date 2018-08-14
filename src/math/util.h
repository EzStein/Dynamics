#ifndef DYNSOLVER_MATH_UTIL_H_
#define DYNSOLVER_MATH_UTIL_H_

#include "math/vector.h"
#include "math/matrix.h"
#include "math/square_matrix.h"
#include "math/matrix_2x2.h"
#include "math/vector2d.h"

#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>
#include <list>
#include <unordered_set>

namespace dynsolver {
namespace math {

// Solves the system of linear equations, matrix * solution = vec
// and returns the result solution as a parameter. If the solution does not
// exist or is not unique (if mat is not invertible) then the function returns
// false and does not alter solution. Requires that matrix.size(), vector.size()
// are the same.
bool solve_linear(const square_matrix& matrix,
		  const vector& vec, vector* solution);

// Returns the subvector formed by extracting in order the elements of vec
// given by the indices.
vector extract_vector(const vector& vec, const std::vector<int>& indices);


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

struct dynamical_point {
  // Represents the dynamical variables in the order of dynamicalVarNames vector
  math::vector vars;

  // The value of the variable of differentiation.
  double t;
};

template<class FUNCTION> 
std::list<dynamical_point> euler_method(const std::vector<FUNCTION>& vectorField,
					const std::vector<int>& indices,
					int varDiff,
					const vector& init,
					double inc,
					double tMin,
					double tMax) {
  math::vector current(init);
  std::list<dynamical_point> points;
  // Fill in forwards
  while(current[varDiff] <= tMax) {
    // Extract the dynamical variables and variable of differentiation.
    math::vector vars(indices.size());
    for(int i = 0; i != indices.size(); ++i) {
      vars[i] = current[indices[i]];
    }
    double t = current[varDiff];
    points.push_back(dynamical_point{vars, t});
    
    math::euler(vectorField, inc, indices, varDiff, current);
  }
  
  current = init;
  
  // Fill in backwards
  while(current[varDiff] >= tMin) {
    // Extract the dynamical variables and variable of differentiation.
    math::vector vars(indices.size());
    for(int i = 0; i != indices.size(); ++i) {
      vars[i] = current[indices[i]];
    }
    double t = current[varDiff];
    points.push_front(dynamical_point{vars, t});
    
    math::euler(vectorField, -inc, indices, varDiff, current);
  }
  
  return points;
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
  assert(varIndices.size() == functions.size());
  assert(varIndices.size() * varIndices.size() == jacobian.size());
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
    //    std::cout << next.to_string() << std::endl;
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
  //  std::cout << "Params: " << input.to_string() << std::endl;
  //  std::cout << "Image: " << (image * -1).to_string() << std::endl;
  //  std::cout << "Jacobian: " << jacobian.to_string() << std::endl;
  if(!solve_linear(jacobian, image, &increment)) return false;
  //  std::cout << "Increment: " << increment.to_string() << std::endl;
  
  // Compute the result as initial + b. Note that we only update the values of
  // initial corresponding with dependent variables.
  for(int i = 0; i != dimension; ++i) {
    input[varIndex[i]] += increment[i];
  }
  return true;
}

// Computes the derivative of the provided function at the input with
// respect to the variable provided.
template<class FUNCTION>
double derivative(const FUNCTION& function,
		  const math::vector& input,
		  int var) {
  math::vector in(input);
  double delta = 0.01;
  in[var] -= delta;
  double a = function(in.data());
  in[var] += 2*delta;
  double b = function(in.data());
  return (b-a) / (2 * delta);
}

// If success is false, then data is the empty list.
// Otherwise, data contains the found path.
struct find_implicit_path_ret {
  std::list<vector> data = std::list<vector>();
  bool success = false;
};

// Computes an implicitly defined path. The system is a system of n - 1
// equations in n unkowns. The jacobian is the its corresponding jacobian (in
// this case, it is an n - 1 by n matrix).
// Init is the initial input to the functions for use in newton's method.
// Indices associates each variable in system/jacobian to its corresponding
// index in the init vector (the init vector may contain more than n entries
// indicating that some entries are constants). Constraint vars indicate
// the set of variables used to in the constraint equation.
// is given by system. We return a list of vectors indicating the path in the
// system variables in the order given by the system variables. This path
// excludes extra "constant" variables defined in the init vector.
template<class FUNCTION> find_implicit_path_ret
find_implicit_path(const std::vector<FUNCTION>& systemIn,
		   const std::vector<FUNCTION>& jacobianIn,
		   const math::vector& init,
		   const std::vector<int>& indices,
		   const std::unordered_set<int>& constraintVars,
		   const double searchRadius,
		   const double searchIncrement,
		   const double constraintRadius,
		   const int span) {
  std::vector<FUNCTION> system(systemIn);

  // We add the constraint equation to the system.
  double currentRadius;
  vector current;
  system.push_back([&](const double* arr)->double{
      double acc = 0;
      for(int var : constraintVars) {
	double val = arr[indices[var]] - current[indices[var]];
	acc += val * val;

      }
      acc -= currentRadius * currentRadius;
      return acc;
    });

  std::vector<FUNCTION> jacobian(jacobianIn);
  // We update the jacobian appropriately.
  for(int var = 0; var != indices.size(); ++var) {
    if(constraintVars.find(var) == constraintVars.end()) {
      jacobian.push_back([&, var](const double*)->double {
	  return 0;
	});
    } else {
      jacobian.push_back([&, var](const double* arr)->double {
	  return 2 * (arr[indices[var]] - current[indices[var]]);
	});
    }
  }
  
  bool found = false;
  math::vector start(init);
  current = init;
  // We peturb the vector slightly.
  for(int var : constraintVars) {
    start[var] += 0.1;
  }
  // We search in a radial direction.
  for(currentRadius = constraintRadius; currentRadius <= searchRadius;
      currentRadius += searchIncrement) {
    found = newton_method(system, jacobian, indices, start);
    if(found) {
      break;
    }
  }
  if(!found) return find_implicit_path_ret();
  
  math::vector prev(start);
  current = start;
  currentRadius = constraintRadius;
  std::list<math::vector> points;
  points.push_back(extract_vector(current, indices));
  bool first = true;
  for(int i = 0; i != span; ++i) {
    math::vector next(current);
    
    // Perturb the initial condition so it is in the direction we expect.
    for(int var : constraintVars) {
      double change = current[indices[var]] - prev[indices[var]];
      if(first) change = 0.1;
      next[indices[var]] += change;
    }

    bool success = newton_method(system, jacobian, indices, next);
    if(!success || (next.distance(current) >= next.distance(prev) && !first)) {
      break;
    }
    first = false;
    
    points.push_back(extract_vector(next, indices));

    prev = current;
    current = next;
  }
  if(points.size() == 1) return find_implicit_path_ret();
    
  current = start;
  prev = start;
  for(int i = 0; i != indices.size(); ++i) {
    prev[indices[i]] = (*(++points.begin()))[i];
  }
  
  for(int i = 0; i != span; ++i) {
    math::vector next(current);
    // Perturb the initial condition so it is in the direction we expect.
    for(int var : constraintVars) {
      next[indices[var]] += current[indices[var]] - prev[indices[var]];
    }
    bool success = newton_method(system, jacobian, indices, next);
    if(!success || next.distance(current) >= next.distance(prev)) {
      break;
    }
    
    points.push_front(extract_vector(next, indices));

    prev = current;
    current = next;
  }
  find_implicit_path_ret ret;
  ret.data = points;
  ret.success = true;
  return ret;
}

// Computes the intersection between the two lines defined as,
// A1 to B1 and A2 to B2.
vector2d intersect(const vector2d& A1, const vector2d& A2,
		   const vector2d& B1, const vector2d& B2);

// Determines whether the points A1, and A2 lie on either side of the
// line B1 to B2.
bool splits(const vector2d& A1, const vector2d& A2,
	    const vector2d& B1, const vector2d& B2);

} // namespace math
} // namespace dynsolver
#endif
