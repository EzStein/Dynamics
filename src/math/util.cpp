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
  NOTE: the size of the initVals vector should be one greater than the
  size of the vecField function.
  The first initVal is the time t parameter and all subsequent ones
  correspond to the elements of vecField.
  The
  number of elements in the scratchSpace should be the
  same as the size of the vecField.
  The return values are left in the initVals section.
  NOTE: the dimension refers to the number of variables not including time.
  initVals should have length dim+1, but vecField should have length dim*/
  void euler(std::vector<double>& initVals,
    const std::vector<driver::double_func_t>& vecField, double inc, double* scratchSpace) {
    assert(vecField.size() + 1 == initVals.size());
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
}
