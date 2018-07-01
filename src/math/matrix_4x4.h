#ifndef DYNSOLVER_MATH_MATRIX_4X4_H_
#define DYNSOLVER_MATH_MATRIX_4X4_H_

#include "math/square_matrix.h"

namespace dynsolver {
namespace math {

class matrix_4x4 : public square_matrix {
public:
  // Constructs the identity matrix
  matrix_4x4();
};

matrix_4x4 operator*(matrix_4x4, const matrix_4x4&);

} // namespace math
} // namespace dynsolver
#endif
