#include "math/matrix.h"

#include <cassert>
#include <cmath>
#include <cstring>
#include <utility>

namespace dynsolver {
namespace math {

matrix::~matrix() {
  delete[] array;
}

matrix::matrix(const matrix& matrix) : rows(matrix.rows), cols(matrix.cols) {
  array = new double[rows*cols];
  std::memcpy(array, matrix.array, rows*cols*sizeof(double));
}

matrix::matrix(matrix&& matrix)
    : rows(matrix.rows), cols(matrix.cols), array(matrix.array) {
  matrix.array = nullptr;
}

matrix& matrix::operator=(const matrix& other) {
  if(&other == this) return *this;
  delete[] array;
  rows = other.rows;
  cols = other.cols;
  array = new double[rows*cols];
  std::memcpy(array, other.array, rows*cols*sizeof(double));
  return *this;
}

matrix& matrix::operator=(matrix&& other) {
  if(&other == this) return *this;
  delete[] array;
  rows = other.rows;
  cols = other.cols;
  array = other.array;
  other.array = nullptr;
  return *this;
}

matrix::matrix(int rows, int cols) : rows(rows), cols(cols) {
  assert(rows > 0 && cols > 0);
  array = new double[rows*cols];
}

matrix::matrix(int rows, int cols, double value) : matrix(rows, cols) {
  for(int row = 0; row != rows; ++row) {
    for(int col = 0; col != cols; ++col) {
      (*this)[row][col] = value;
    }
  }
}

int matrix::get_rows() const {
  return rows;
}

int matrix::get_cols() const {
  return cols;
}

// NOTE: this may be inefficient for large arrays do to caching problems.
void matrix::transpose() {
  std::swap(rows, cols);
  for(int r = 0; r != rows; ++r) {
    for(int c = 0; c != cols; ++c) {
      std::swap(array[r*cols + c], array[c*rows + r]);
    }
  }
  std::swap(rows, cols);
}

double matrix::norm() const {
  double sum = 0;
  for(int r = 0; r != rows; ++r) {
    for(int c = 0; c != cols; ++c) {
      double entry = (*this)[r][c];
      sum += entry*entry;
    }
  }
  return std::sqrt(sum);
}

const double* matrix::data() const {
  return array;
}

double* matrix::operator[](int row) {
  return array + row * cols;
}

const double* matrix::operator[](int row) const {
  return array + row * cols;
}

void matrix::set_zeros() {
  for(int row = 0; row != rows; ++row) {
    for(int col = 0; col != cols; ++col) {
      if(std::abs((*this)[row][col]) < kTolerance) {
        (*this)[row][col] = 0.0;
      }
    }
  }
}

void matrix::set_ones() {
  for(int row = 0; row != rows; ++row) {
    for(int col = 0; col != cols; ++col) {
      if(std::abs((*this)[row][col] - 1.0) < kTolerance)
        (*this)[row][col] = 1;
    }
  }
}

void matrix::rref() {
  double det;
  rref(det);
}


void matrix::operator+=(const matrix& other) {
  assert(rows == other.rows);
  assert(cols == other.cols);
  for(int r = 0; r != rows; ++r) {
    for(int c = 0; c != cols; ++c) {
      (*this)[r][c] += other[r][c];
    }
  }
}

void matrix::operator-=(const matrix& other) {
  assert(rows == other.rows);
  assert(cols == other.cols);
  for(int r = 0; r != rows; ++r) {
    for(int c = 0; c != cols; ++c) {
      (*this)[r][c] -= other[r][c];
    }
  }
}


void matrix::operator*=(const matrix& other) {
  assert(cols == other.rows);
  double* newArray = new double[rows*other.cols];

  for(int row = 0; row != rows; ++row) {
    for(int col = 0; col != other.cols; ++col) {
      newArray[row*other.cols + col] = 0;
      for(int i = 0; i != cols; ++i)
        newArray[row*other.cols + col] += (*this)[row][i]*other[i][col];
    }
  }
  delete[] array;
  array = newArray;
  cols = other.cols;
}

void matrix::operator*=(double scalar) {
  for(int r = 0; r != rows; ++r) {
    for(int c = 0; c != cols; ++c) {
      (*this)[r][c] *= scalar;
    }
  }
}

void matrix::operator/=(double scalar) {
  for(int r = 0; r != rows; ++r) {
    for(int c = 0; c != cols; ++c) {
      (*this)[r][c] /= scalar;
    }
  }
}

void matrix::operator-() {
  operator*=(-1.0);
}

//
// This implementation treats zeros as follows:
// Any number that is within tolerance of zero will be
// treated as though it were a zero. However, for optimization
// reasons, it will not be replaced with zero, but will be treated
// as being equal to all values also within that tolerance.
// Thus it may be useful to call, set_zeros() after calling
// this method. The method will also calculate the determinant
//
void matrix::rref(double& determinant) {
  determinant = 1.0;

  /*The first step in RREF is to choose a (nonzero) pivot in the first column if one exists
    We will choose the largest pivot in std::absolute value in order to minimize instability*/

  /*The rowStart indicates where we should start searching for a pivot.
    That is, we ignore all rows before rowStart (above it in the matrix)*/
  int rowStart = 0;

  /*
   * We apply this process of finding a pivot to each row,
   * although the rowStart may not increment each time (for instance
   if we don't find a pivot)
  */
  for(int pivotColumn = 0; pivotColumn != cols; ++pivotColumn) {
    /*Holds the row index (not the value) of the largest pivot in std::absolute value in the column.
      It is initially set to rowStart*/
    int largestPivotIndex = rowStart;

    /*We traverse the column. We start at rowStart + 1 since the largestPivotIndex is already set to rowStart*/
    for(int pivotChoice = rowStart + 1; pivotChoice != rows; ++pivotChoice) {
      if(std::abs(static_cast<double>((*this)[largestPivotIndex][pivotColumn]))
         < std::abs(static_cast<double>((*this)[pivotChoice][pivotColumn])))
        largestPivotIndex = pivotChoice;
    }
    /*If after the traversal, largestPivotIndex points to a zero element, then the whole column is zero,
      so we may skip it and move on to the next column without incrementing rowStart.
      Since we are doing a comparison by zero,
      we will use the tolerance*/
    if(std::abs((*this)[largestPivotIndex][pivotColumn]) < kTolerance) continue;


    /*Otherwise, We interchange it with the rowStart row. This is a potentially
      expensive operation (since we are using a 2d array and not an array of points) thus an adjacency
      list might be more efficient than a 2d array in this case*/
    double largestPivotValue = (*this)[largestPivotIndex][pivotColumn];
    /*In this case, we are multiplying the row by 1/largestPivotValue,
      so we multiply by the inverse (largestPivotValue) for the determinant*/
    determinant *= largestPivotValue;

    if(largestPivotIndex != rowStart) {
      /*If the largestPivotIndex is not already in the start row then:*/
      /*We can start with the pivot column because all values to the left should already be
        effectively zero*/
      /*We multiply the determinant by -1 for the swap*/
      determinant *= -1;
      for(int i = pivotColumn; i != cols; ++i) {
        /*Performs the swap and division*/
        double tmp = (*this)[rowStart][i];
        (*this)[rowStart][i] = (*this)[largestPivotIndex][i]/largestPivotValue;
        (*this)[largestPivotIndex][i] = tmp;

      }
    } else {
      /*In this case, we don't need to perform the
        swap, just the division*/
      for(int i = pivotColumn; i != cols; ++i) {
        (*this)[rowStart][i] = (*this)[largestPivotIndex][i] / largestPivotValue;

      }
    }


    /*Now for all rows below rowStart, we subtract the pivot
      row multiplied by the first element in the examined row*/
    for(int examinedRow = rowStart + 1; examinedRow != rows; ++examinedRow) {
      double multiplier = (*this)[examinedRow][pivotColumn];
      /*This value is known to be zero*/
      (*this)[examinedRow][pivotColumn] = 0;
      for(int examinedColumn = pivotColumn + 1; examinedColumn != cols; ++examinedColumn) {
        (*this)[examinedRow][examinedColumn] -= multiplier*(*this)[rowStart][examinedColumn];
      }
    }

    /*Now, we can increment the rowStart*/
    ++rowStart;

    /*We are done with the first phase*/
    if(rowStart == rows) break;
  }
  if(rowStart != rows) {
    /*rowStart counts the number of pivots. If the matrix
      is square, and rowStart != rows, then it does not reduce to the identity,
      so the determinant is 0.
    */
    determinant = 0;
  }

  /*In the second phase, we start with the last non zero row.
    We use its pivot (first nonzero value) to form zeros in the column above it*/

  for(rowStart = rows - 1; rowStart != -1; --rowStart) {
    for(int pivotColumn = 0; pivotColumn != cols; ++pivotColumn) {
      if(std::abs((*this)[rowStart][pivotColumn]) >= kTolerance) {
        /*We have found the first nonzero entry in the row*/
        /*We now subtract this row times a multiplier from all the above rows*/
        for(int examinedRow = 0; examinedRow != rowStart; ++examinedRow) {
          /*We can ignore previous values since they are zero*/
          double multiplier = (*this)[examinedRow][pivotColumn];
          /*We know that the examined element will become zero*/
          (*this)[examinedRow][pivotColumn] = 0;
          for(int examinedColumn = pivotColumn+1; examinedColumn != cols; ++examinedColumn) {
            (*this)[examinedRow][examinedColumn] -= multiplier*(*this)[rowStart][examinedColumn];
          }
        }
        break;
      }
    }
  }
}

matrix matrix::adjoin_by_row(const matrix& mat1, const matrix& mat2) {
  assert(mat1.get_rows() == mat2.get_rows());
  int rows = mat1.get_rows();
  int cols1 = mat1.get_cols();
  int cols2 = mat2.get_cols();
  matrix retMat(rows, cols1 + cols2);
  for(int row = 0; row != rows; ++row) {
    for(int col = 0; col != cols1 + cols2; ++col) {
      if(col < cols1)
        retMat[row][col] = mat1[row][col];
      else
        retMat[row][col] = mat2[row][col-cols1];
    }
  }
  return retMat;
}

matrix matrix::adjoin_by_column(const matrix& mat1, const matrix& mat2) {
  assert(mat1.get_cols() == mat2.get_cols());
  int cols = mat1.get_cols();
  int rows1 = mat1.get_rows();
  int rows2 = mat2.get_rows();
  matrix retMat(rows1 + rows2, cols);
  for(int row = 0; row != rows1 + rows2; ++row) {
    for(int col = 0; col != cols; ++col) {
      if(row < rows1)
        retMat[row][col] = mat1[row][col];
      else
        retMat[row][col] = mat2[row - rows1][col];
    }
  }
  return retMat;
}

matrix operator+(matrix lhs, const matrix& rhs) {
  lhs += rhs;
  return lhs;
}

matrix operator-(matrix lhs, const matrix& rhs) {
  lhs -= rhs;
  return lhs;
}

matrix operator*(matrix lhs, const matrix& rhs) {
  lhs *= rhs;
  return lhs;
}

matrix operator*(double scal, matrix mat) {
  mat *= scal;
  return mat;
}

matrix operator*(matrix mat, double scal) {
  mat *= scal;
  return mat;
}

matrix operator/(matrix mat, double scal) {
  mat /= scal;
  return mat;
}
} // namespace math
} // namespace dynsolver
