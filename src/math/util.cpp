#include "math/util.h"
#include <iostream>
namespace math {
  static_vector<double, 2> pixel_to_value(const static_vector<int, 2>& pixel, const static_vector<int, 2>& canvasSize,
    const static_vector<double, 2>& valueBoundaryTopLeft, const static_vector<double, 2>& valueBoundaryBottomRight) {

    double scaling = (valueBoundaryBottomRight[0] - valueBoundaryTopLeft[0])/canvasSize[0];
    static_matrix<double, 2, 2> mat;
    mat[0][0] = scaling;
    mat[0][1] = 0;
    mat[1][0] = 0;
    mat[1][1] = -scaling;
    return valueBoundaryTopLeft + mat * static_vector<double, 2>(pixel);
  }

  static_vector<int, 2> value_to_pixel(const static_vector<double, 2>& value, const static_vector<int, 2>& canvasSize,
    const static_vector<double, 2>& valueBoundaryTopLeft, const static_vector<double, 2>& valueBoundaryBottomRight) {

    double scaling = (valueBoundaryBottomRight[0] - valueBoundaryTopLeft[0])/canvasSize[0];
    static_matrix<double, 2, 2> mat;
    mat[0][0] = 1.0/scaling;
    mat[0][1] = 0;
    mat[1][0] = 0;
    mat[1][1] = -1.0/scaling;
    return static_vector<int, 2>(mat * (value - valueBoundaryTopLeft));
  }

  /*Like the previous two methods except it uses the pixelToValueRatio
  and a left corner in its calculations*/
  static_vector<double, 2> pixel_to_value(const static_vector<int, 2>& pixel,
    const static_vector<double, 2>& valueBoundaryTopLeft, const static_vector<double, 2>& pixelToValueRatio) {
      static_vector<double, 2> ret;
      ret[0] = pixel[0]/pixelToValueRatio[0] + valueBoundaryTopLeft[0];
      ret[1] = valueBoundaryTopLeft[1] - pixel[1]/pixelToValueRatio[1];
      return ret;
  }

  static_vector<int, 2> value_to_pixel(const static_vector<double, 2>& value,
    const static_vector<double, 2>& valueBoundaryTopLeft, const static_vector<double, 2>& pixelToValueRatio) {
    static_vector<int, 2> ret;
    ret[0] = static_cast<int>((value[0] - valueBoundaryTopLeft[0])*pixelToValueRatio[0]);
    ret[1] = static_cast<int>(-(value[1] - valueBoundaryTopLeft[1])*pixelToValueRatio[1]);
    return ret;
  }

  /*Performs euler's method on an
  arbitrary dimension.
  The inputs are a std::vector of doubles specifying the starting value,
  a std::vector of functions specifying the vector field in each dimension,
  the increment size, and
  an optional scratchSpace area may be provided, if speed is desired.
  This prevents allocating and deallocating small arrays very often.
  NOTE: the size of the initVals vector should be at least one greater than the
  size of the vecField function. If it is more than this, then the trailing
  entries are considered parameters and are not altered by this function.
  The first initVal is the time t and all subsequent ones
  correspond to the elements of vecField followed by parameters.
  The
  number of elements in the scratchSpace should be the
  same as the size of the vecField.
  The return values are left in the initVals section.
  NOTE: the dimension refers to the number of variables not including time.
  initVals should have length at least dim+1, but vecField should have length dim*/
  void euler(math::vector<double>& initVals,
    const std::vector<driver::double_func_t>& vecField, double inc, double* scratchSpace) {
    assert(vecField.size() + 1 <= initVals.size());
    int dimension = vecField.size();
    double* increments;
    if(!scratchSpace)
      increments = new double[dimension];
    else
      increments = scratchSpace;


    for(int i = 0; i != dimension; ++i) {
      increments[i] = inc * vecField[i](initVals.data());
    }
    initVals[0] += inc;
    for(int i = 0; i != dimension; ++i) {
      initVals[i+1] += increments[i];
    }

    if(!scratchSpace)
      delete[] increments;
  }

  /*
  * performs one round of newtons method iteration
  * on an arbitrary dimension. The input is a vector valued function
  * given as a pointer to an array driver::double_func_t as well
  as its jacobian, which is given as a pointer to the 2d
  array of driver::double_func_t specified in row major order.
  Additionally, the initial condition is specified as a math::vector<double>
  which is overwritten with the final value after one iteration.
  Note that the init vector should also include any parameters that need
  to be passed to the function, but which are not variables to solve for.
  * Returns true if the iteration was successful (the jacobian was non-singular)
  and false otherwise.
  * if insertZeroTime is true, then a time parameter of zero is inserted
  * before the other parameters when a compiled function is called.
  */
  bool newton_raphson_iteration(
  math::vector<double>& init, const std::vector<driver::double_func_t>& functions,
  const std::vector<driver::double_func_t>& jacobian, bool insertZeroTime) {
    int dimension = functions.size();
    math::vector<double> parameters(init.size() + 1);
    if(insertZeroTime) {
      parameters[0] = 0.0;
      for(int i = 1; i != parameters.size(); ++i) {
        parameters[i] = init[i - 1];
      }
    } else {
      for(int i = 0; i != init.size(); ++i) {
        parameters[i] = init[i];
      }
    }

    /*We first compute the jacobian matrix*/
    matrix<double> jacobianMatrix(dimension,dimension);
    for(int r = 0; r != dimension; ++r) {
      for(int c = 0; c != dimension; ++c) {
        jacobianMatrix[r][c] = jacobian[r*dimension + c](parameters.data());
      }
    }
    /*We now compute the image of the init condition under the function and negate it.
    This is used to solve a linear system*/
    vector<double> vec(dimension);
    for(int i = 0; i != dimension; ++i) {
      vec[i] = -1 * functions[i](parameters.data());
    }

    vector<double> sol(init.size(), 0);
    if(!solve_uniquely(jacobianMatrix, vec, sol)) return false;
    init += sol;
    return true;
  }
}
