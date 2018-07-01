#ifndef DYNSOLVER_MATH_MATRIX_3X3_H_
#define DYNSOLVER_MATH_MATRIX_3X3_H_

#include "math/square_matrix.h"

namespace dynsolver {
namespace math {

class vector3d;
class matrix_4x4;

class matrix_3x3 : public square_matrix {
public:
  // Constructs the identity matrix.
  matrix_3x3();
  
  // Augmenst this matrix to a 4x4 matrix.
  matrix_4x4 augment_to_4x4() const;
};

matrix_3x3 operator*(matrix_3x3, const matrix_3x3&);
vector3d operator*(matrix_3x3, const vector3d&);
} // namespace math
} // namespace dynsolver
#endif
