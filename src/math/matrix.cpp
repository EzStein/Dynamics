#include "math/matrix.h"

#include <cassert>
#include <cmath>
#include <cstring>
#include <sstream>
#include <utility>
#include <iostream>
#include "math/eigenvalue.h"

#include "math/vector.h"

namespace dynsolver {
namespace math {

matrix::matrix() : matrix(1, 1, 0.0) { }

matrix::~matrix() {
  delete[] array;
}

matrix::matrix(const std::vector<vector>& vectors) {
  assert(vectors.size() >= 1);
  cols = vectors.size();
  rows = vectors[0].size();
  array = new double[rows * cols];
  for(int r = 0; r != rows; ++r) {
    for(int c = 0; c != cols; ++c) {
      (*this)[r][c] = vectors[c][r];
    }
  }
}

matrix::matrix(const matrix& matrix) : rows(matrix.rows), cols(matrix.cols) {
  if(matrix.array == nullptr) {
    array = nullptr;
  } else {
    array = new double[rows*cols];
    std::memcpy(array, matrix.array, rows*cols*sizeof(double));
  }
}

matrix::matrix(matrix&& mat)
  : array(mat.array), rows(mat.rows), cols(mat.cols) {
  mat.array = nullptr;
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

void matrix::as_float_array(float* arr) const {
  for(int i = 0; i != rows * cols; ++i) {
    arr[i] = static_cast<float>(array[i]);
  }
}

void matrix::normalize() {
  *this /= norm();
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
  assert(0 <= row && row < rows);
  return array + row * cols;
}

const double* matrix::operator[](int row) const {
  assert(0 <= row && row < rows);
  return array + row * cols;
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

void matrix::split_vertically(int leftCols, matrix* left, matrix* right) const {
  assert(leftCols > 0 && leftCols < cols);
  matrix lMatrix(rows, leftCols);
  matrix rMatrix(rows, cols - leftCols);
  for(int r = 0; r != rows; ++r) {
    for(int c = 0; c != leftCols; ++c) {
      lMatrix[r][c] = (*this)[r][c];
    }
  }
  for(int r = 0; r != rows; ++r) {
    for(int c = leftCols; c != cols; ++c) {
      rMatrix[r][c - leftCols] = (*this)[r][c];
    }
  }
  *left = lMatrix;
  *right = rMatrix;
}

bool matrix::is_identity() const {
  if(rows != cols) return false;
  for(int r = 0; r != rows; ++r) {
    for(int c = 0; c != cols; ++c) {
      double val = r == c ? 1 : 0;
      if(std::abs((*this)[r][c] - val) > kTolerance) {
	return false;
      }
    }
  }
  return true;
}

std::vector<vector> matrix::null_space() const {
  matrix copy(*this);
  copy.rref();
  
  // This vectors holds the pivot column for the associated row.
  // That is, pivotCol[row] is the associated pivotCol.
  std::vector<int> pivotCol;
  std::vector<vector> basis;
  int r = 0;
  for(int c = 0; c != cols; ++c) {
    if(std::abs(copy[r][c]) < kTolerance) {
      // We have found a non pivot column
      vector vec(cols, 0.0);
      vec[r] = 1;
      for(int i = 0; i != r; ++i) {
	vec[pivotCol[i]] = -copy[i][c];
      }
      vec.normalize();
      basis.push_back(vec);
    } else {
      // We have found a pivot column
      pivotCol.push_back(c);
      ++r;
    }
  }
  return basis;
}

//
// This implementation treats zeros as follows:
// Any number that is within tolerance of zero will be
// treated as though it were a zero. However, for optimization
// reasons, it will not be replaced with zero, but will be treated
// as being equal to all values also within that tolerance.
// Thus it may be useful to call, set_zeros() after calling
// this method. The method will also calculate the determinant
double matrix::rref() {
  double determinant = 1.0;
  
  // The first step in RREF is to choose a (nonzero) pivot in the first
  // column if one exists.  We will choose the largest pivot in
  // std::absolute value in order to minimize instability 

  // The rowStart indicates where we should start searching for a pivot.
  // That is, we ignore all rows before rowStart (above it in the matrix)
  int rowStart = 0;

  // We apply this process of finding a pivot to each row,
  // although the rowStart may not increment each time (for instance
  // we don't find a pivot)
  for(int pivotColumn = 0; pivotColumn != cols; ++pivotColumn) {
    // Holds the row index (not the value) of the largest pivot in
    // std::absolute value in the column. It is initially set to rowStart
    int largestPivotIndex = rowStart;

    // We traverse the column. We start at rowStart + 1 since the
    // largestPivotIndex is already set to rowStart
    for(int pivotChoice = rowStart + 1; pivotChoice != rows; ++pivotChoice) {
      if(std::abs(static_cast<double>((*this)[largestPivotIndex][pivotColumn]))
         < std::abs(static_cast<double>((*this)[pivotChoice][pivotColumn])))
        largestPivotIndex = pivotChoice;
    }
    // If after the traversal, largestPivotIndex points to a zero
    // element, then the whole column is zero,
    //  so we may skip it and move on to the next column without incrementing
    // rowStart.  Since we are doing a comparison by zero,
    //  we will use the tolerance
    if(std::abs((*this)[largestPivotIndex][pivotColumn]) < kTolerance) continue;


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
  return determinant;
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

std::string matrix::to_string() const {
  std::stringstream sstream;
  for(int r = 0; r != rows; ++r) {
    for(int c = 0; c != cols; ++c) {
      sstream << (*this)[r][c] << " ";
    }
    sstream << "\n";
  }
  return sstream.str();
}

vector matrix::get_column(int c) const {
  vector vec(rows);
  for(int r = 0; r != rows; ++r) {
    vec[r] = (*this)[r][c];
  }
  return vec;
}

// We implement qr_decomposition with the gram schmidt process.
void matrix::qr_decomposition(matrix& qMat, matrix& rMat) const {
  qMat = matrix(rows, cols);
  rMat = matrix(cols, cols, 0.0);
  // Holds the list of computed orthogonal basis vectors.
  std::vector<vector> projections;
  // Same as the projection vector except each vector is normalized.
  std::vector<vector> normals;
  projections.push_back(get_column(0));
  for(int c = 1; c != cols; ++c) {
    vector acc(get_column(c));
    for(int i = 0; i != c; ++i) {
      acc = acc - acc.projection(projections[i]);
    }
    projections.push_back(acc);
    vector tmp(acc);
    tmp.normalize();
    projections.push_back(tmp);
    for(int r = 0; r != c; ++r) {
      rMat[r][c] = vector::dot(normals[r], projections[c]);
    }
  }
  qMat = matrix(normals);
}


matrix matrix::operator+(const matrix& rhs) const {
  matrix mat(*this);
  mat += rhs;
  return mat;
}

matrix matrix::operator-(const matrix& rhs) const {
  matrix mat(*this);
  mat -= rhs;
  return mat;
}

matrix matrix::operator*(const matrix& rhs) const {
  matrix mat(*this);
  mat *= rhs;
  return mat;
}

matrix matrix::operator*(double scal) const {
  matrix mat(*this);
  mat *= scal;
  return mat;
}

matrix matrix::operator/(double scal) const {
  matrix mat(*this);
  mat /= scal;
  return mat;
}

vector matrix::operator*(const vector& vec) const {
  matrix mat(*this);
  mat *= vec;
  vector ret(vec.size());
  for(int i = 0; i != vec.size(); ++i) {
    ret[i] = mat[i][0];
  }
  return ret;
}
} // namespace math
} // namespace dynsolver
