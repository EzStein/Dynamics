#ifndef DYNSOLVER_MATH_MATRIX_4X4_H_
#define DYNSOLVER_MATH_MATRIX_4X4_H_

#include "math/square_matrix.h"

namespace dynsolver {
namespace math {

class matrix_4x4 : public square_matrix {
public:
  // Constructs the identity matrix
  matrix_4x4();

  // Generates a 4x4 translation matrix which translates, just the
  // x and y coordinates.
  static matrix_4x4 translation(double x, double y);

  // Generates a 4x4 scaling matrix which scales, just the
  // x and y coordinates.
  static matrix_4x4 scale(double x, double y);
  
  // Generates a 4x4 translation matrix which translates the x y and z
  // coordinates.
  static matrix_4x4 translation(double x, double y, double z);

  // Generates a 4x4 scaling matrix which scales, just the
  // x, y and z coordinates.
  static matrix_4x4 scale(double x, double y, double z);
};

matrix_4x4 operator*(matrix_4x4, const matrix_4x4&);

} // namespace math
} // namespace dynsolver
#endif
