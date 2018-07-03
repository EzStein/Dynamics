#ifndef DYNSOLVER_MATH_MATRIX_H_
#define DYNSOLVER_MATH_MATRIX_H_

#include <cstddef>
#include <string>

namespace dynsolver {
namespace math {

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
  // Default constructor builds a 0 by 0 matrix.
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

  // Returns the number of rows and columns of the matrix.
  int get_rows() const;
  int get_cols() const;

  // Transposes the matrix.
  // The transpose occurs in place and mutates the state of this matrix.
  matrix& transpose();

  // Returns the standard frobenius matrix norm
  // which is the square root of the sum of the squares of all entries.
  double norm() const;

  // Returns the a const pointer to the data contained in this matrix.
  const double* data() const;

  // Converts this matrix into a float array in ROW major fasion.
  // To get a double array version simply use data().
  void as_float_array(float* arr) const;

  // Returns a pointer to the start of the row.
  double* operator[](int);
  const double* operator[](int) const;

  // Adds or subtracts the provided matrix from this one.
  // Requires that the matrices have the same dimensions.
  matrix& operator+=(const matrix&);
  matrix& operator-=(const matrix&);

  // Right multiples this matrix by the provided one. The operation may require
  // memory reallocation if the new matrix is of a larger size.
  // Requires that the cols of this matrix are the same number as the rows of
  // the other matrix.
  matrix& operator*=(const matrix&);

  // Scalar multiplies this matrix by the provided number.
  matrix& operator*=(double);

  // Scalar divides this matrix by the provided number.
  matrix& operator/=(double);

  // Row reduces this matrix and returns the computed determinant through a
  // pointer. Note that the returned value is only meaningful for square
  // matrices. If the passed pointer is null, it is ignored.
  matrix& rref(double* det = nullptr);

  // Splits the matrix vectically along some column. Returns the left and right,
  // parts through pointers. The left part has cols number of columns.
  void split_vertically(int cols, matrix* left, matrix* right);

  // Returns true if this is the square identity matrix.
  // Note that values within the tolerance of 0 or 1 are
  // considered to be zero or 1.
  bool is_identity();

  // Pretty prints this matrix to a string.
  std::string to_string() const;

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

// This namespace is used to refer to the more base overloads of these
// operators when the compiler by default chooses more derived versions.
namespace matrix_ops {
// Various arithmetic operations on matrices
matrix operator+(matrix, const matrix&);
matrix operator-(matrix, const matrix&);
matrix operator*(matrix, const matrix&);
matrix operator*(double, matrix);
matrix operator*(matrix, double);
matrix operator/(matrix, double);
} // namespace matrix_ops
} // namespace math
} // namespace dynsolver
#endif
