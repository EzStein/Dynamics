#ifndef DYNSOLVER_MATH_SQUARE_MATRIX_H_
#define DYNSOLVER_MATH_SQUARE_MATRIX_H_
#include "math/matrix.h"
namespace dynsolver {
namespace math {

// A specialization of the matrix class that represents a square matrix.
class square_matrix : public matrix {
 public:
  // Constructs a 0x0 empty matrix.
  square_matrix();
  
  // Constructs a square matrix where size is the number of rows and columns.
  square_matrix(int size);

  // Constructs a square matrix where all entries are initialized to value.
  square_matrix(int size, double value);

  // Returns the number of rows/cols of the matrix.
  int size() const;
  
  // Computes the determinant of this matrix.
  double determinant() const;

  // Returns true if the matrix is invertible.
  bool invertable() const;

  // Inverts this matrix if possible. If the matrix is not invertiable,
  // returns false through the pointer. If the pointer is null, it is
  // ignored.
  // Note that if the matrix is not invertible, this function does not alter
  // matrix.
  square_matrix& invert(bool* success = nullptr);

  square_matrix& operator+=(const square_matrix&);
  square_matrix& operator-=(const square_matrix&);
  square_matrix& operator*=(const square_matrix&);
  square_matrix& operator*=(double);
  square_matrix& operator/=(double);

  // Returns the identity matrix of dimension n.
  // Requires that n is positive.
  static square_matrix identity_matrix(int n);
};

namespace square_matrix_ops {
// These functions perform the arithmatic in the expected way on square matrices.
square_matrix operator+(square_matrix, const square_matrix&);
square_matrix operator-(square_matrix, const square_matrix&);
square_matrix operator*(square_matrix, const square_matrix&);
square_matrix operator*(double, square_matrix);
square_matrix operator*(square_matrix, double);
square_matrix operator/(square_matrix, double);
} // square_matrix_ops
} // namespace math
} // namespace dynsolver
#endif
