#ifndef DYNSOLVER_MATH_MATRIX_H_
#define DYNSOLVER_MATH_MATRIX_H_

#include <cstddef>
#include <string>
#include <vector>

namespace dynsolver {
namespace math {
class vector;
class eigenvalue;

// This class represents a potentially rectangular matrix of doubles.
// It provides numerical operations on matrices that are specifically optimized
// double manipulation. The matrix class is mutable.
// Elements of the matrix may be accessed using the 2-D array syntax.
// Example: the first element of the first row is matrix[0][0]. The second
// element of the first row is matrix[0][1]
class matrix {
 protected:
  // A ponter to an array which stores elements in ROW MAJOR
  // The elements of the same row are contiguous in memory. It is
  // thus always faster to traverse the matrix row by row.
  // The item in the rth row and cth column is array[r * cols + c]
  double* array;

  // The number of rows and columns of the matrix.
  // Class Invariant: rows and columns are both positive.
  size_t rows, cols;

  static constexpr double kTolerance = 1.0e-10;
  
 public:
  // Default constructor builds a 0 by 0 matrix. Note that this function does
  // not allocate any memory. It is an error to call any functions while the
  // matrix is in this state.
  matrix();
  
  // Destructor
  ~matrix();

  // Copy Constructor
  matrix(const matrix&);

  // Move Constructor
  matrix(matrix&&);

  // Copy Assignment
  matrix& operator=(const matrix&);

  // Move Assignment
  matrix& operator=(matrix&&);

  // Creates a matrix with the appropriate number of rows and columns.
  // All elements in the matrix are initialized to the given value.
  // Requires: rows and cols are positive integers. 
  matrix(int rows, int cols, double value);

  // Creates an instance of a matrix with the appropriate number of rows and
  // columns. Does not initialize any entries in the matrix.
  // Requires: rows and cols are positive integers.
  matrix(int rows, int cols);

  // Constructs the matrix from a list of column vectors.
  matrix(const std::vector<vector>&);

  // Returns the number of rows and columns of the matrix.
  int get_rows() const;
  int get_cols() const;

  // Transposes the matrix.
  // The transpose occurs in place and mutates the state of this matrix.
  void transpose();

  // Returns the standard frobenius matrix norm
  // which is the square root of the sum of the squares of all entries.
  double norm() const;

  // Normalizes the matrix by performing this /= norm();
  void normalize();

  // Returns the a const pointer to the data contained in this matrix.
  const double* data() const;

  // Converts this matrix into a float array in ROW major fasion.
  // To get a double array version simply use data().
  void as_float_array(float* arr) const;

  // Returns a pointer to the start of the row.
  // These functions do not perform any bound checking.
  double* operator[](int);
  const double* operator[](int) const;

  // Adds or subtracts the provided matrix from this one.
  // Requires that the matrices have the same dimensions.
  void operator+=(const matrix&);
  void operator-=(const matrix&);

  // Right multiples this matrix by the provided one. The operation may require
  // memory reallocation if the new matrix is of a larger size.
  // Requires that the cols of this matrix are the same number as the rows of
  // the other matrix.
  void operator*=(const matrix&);

  // Scalar multiplies this matrix by the provided number.
  void operator*=(double);

  // Scalar divides this matrix by the provided number.
  void operator/=(double);

  // Row reduces this matrix and returns the computed determinant.
  // Note that the returned value is only meaningful for square
  // matrices. If the passed pointer is null, it is ignored.
  double rref();

  // Splits the matrix vectically along some column. Returns the left and right,
  // parts through pointers. The left part has cols number of columns.
  void split_vertically(int cols, matrix* left, matrix* right) const;

  // Returns true if this is the square identity matrix.
  // Note that values within the tolerance of 0 or 1 are
  // considered to be zero or 1.
  bool is_identity() const;

  // Decomposes this matrix into an orthogonal matrix q and and an upper
  // triangular matrix r such that q*r is approximately (*this).
  void qr_decomposition(matrix& q, matrix& r) const;

  // Pretty prints this matrix to a string.
  std::string to_string() const;

  // Returns the column vector associated with the given column.
  vector get_column(int col) const;

  // Various arithmetic operations on matrices
  matrix operator+(const matrix&) const;
  matrix operator-(const matrix&) const;
  matrix operator*(const matrix&) const;
  matrix operator*(double) const;
  matrix operator/(double) const;
  vector operator*(const vector&) const;

  // Adjoins the matrices by column. That is,
  // the nth col of the returned matrix is the nth col of
  // the first matrix then the nth col of the second matrix.
  // Requires that mat1.get_cols() == mat2.get_cols()
  static matrix adjoin_by_column(const matrix& mat1, const matrix& mat2);


  // Adjoins the matrices by row. That is,
  // it the nth row of the returned matrix is the nth row of
  // the first matrix then the nth row of the second matrix.
  // Requires that mat1.get_rows() == mat2.get_rows()
  static matrix adjoin_by_row(const matrix& mat1, const matrix& mat2);
};
} // namespace math
} // namespace dynsolver
#endif
