#ifndef DYNSOLVER_MATH_POLY_MATRIX_H_
#define DYNSOLVER_MATH_POLY_MATRIX_H_

#include <cstddef>

namespace dynsolver {
namespace math {

class polynomial;

// This class represents a square matrix of polynomials.
class poly_matrix {
private:
  // The size of this array is size * size and it represents the
  // matrix in ROW major order.
  polynomial* entries;

  // This is the number of rows and columns.
  size_t size;
  
public:
  // Constructs the 1x1 matrix containing the 0 polynomial.
  poly_matrix();

  // Constructs the size x size matrix of 0 polynomials.
  poly_matrix(size_t size);
    
  poly_matrix(const poly_matrix&);
  poly_matrix(poly_matrix&&);

  poly_matrix& operator=(const poly_matrix&);
  poly_matrix& operator=(poly_matrix&&);

  ~poly_matrix();

  // Returns the minor matrix obtained by eliminanting the given row and
  // column. The minor matrix has size one less than the current matrix.
  poly_matrix get_minor(size_t row, size_t col) const;

  // Returns the pointer to the start of the requested row.
  const polynomial* operator[](size_t row) const;
  polynomial* operator[](size_t row);

  // Computes the determinant polynomial.
  polynomial determinant() const;

private:
  // Row reduces this square matrix until it is upper triangular.
  // Then computes and returns the determinant.
  polynomial determinant();
};

} // namespace math
} // namespace dynsolver


#endif
