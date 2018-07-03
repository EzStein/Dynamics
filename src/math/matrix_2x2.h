#ifndef DYNSOLVER_MATH_MATRIX_2X2_H_
#define DYNSOLVER_MATH_MATRIX_2X2_H_

#include "math/square_matrix.h"

namespace dynsolver {
namespace math {

class vector2d;

class matrix_2x2 : public square_matrix {
public:
  // Constructs the 2x2 identity matrix.
  matrix_2x2();
  matrix_2x2 operator*(const matrix_2x2&) const;
  vector2d operator*(const vector2d&) const;
};



} // namespace math
} // namespace dynsolver
#endif
