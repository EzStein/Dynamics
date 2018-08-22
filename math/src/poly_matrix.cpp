#include "math/poly_matrix.h"
#include "math/polynomial.h"

namespace math {

poly_matrix::poly_matrix(size_t size) : entries(nullptr), size(size) {
  entries = new polynomial[size * size];
}

poly_matrix::poly_matrix() : poly_matrix(0) { }
			     
poly_matrix::~poly_matrix() {
  delete[] entries;
}

poly_matrix::poly_matrix(const poly_matrix& other)
  : entries(nullptr), size(other.size) {
  entries = new polynomial[size * size];
  for(int i = 0; i != size * size; ++i) {
    entries[i] = other.entries[i];
  }
}

poly_matrix::poly_matrix(poly_matrix&& other)
  : entries(other.entries), size(other.size) {
  other.entries = nullptr;
}

poly_matrix& poly_matrix::operator=(const poly_matrix& other) {
  if(this == &other) return *this;
  delete[] entries;
  size = other.size;
  entries = new polynomial[size * size];
  for(int i = 0; i != size * size; ++i) {
    entries[i] = other.entries[i];
  }
  return *this;
}

poly_matrix& poly_matrix::operator=(poly_matrix&& other) {
  if(this == &other) return *this;
  delete[] entries;
  entries = other.entries;
  size = other.size;
  other.entries = nullptr;
  return *this;
}

const polynomial* poly_matrix::operator[](size_t row) const {
  return entries + row * size;
}

polynomial* poly_matrix::operator[](size_t row) {
  return entries + row * size;
}

poly_matrix poly_matrix::get_minor(size_t rowSkip, size_t colSkip) const {
  poly_matrix mat(size - 1);
  size_t r(0);
  size_t c(0);
  for(int row = 0; row != size; ++row) {
    if(row == rowSkip) continue;
    for(int col = 0; col != size; ++col) {
      if(col == colSkip) continue;
      mat[r++][c++] = (*this)[row][col];
    }
    c = 0;
  }
  return mat;
}

// We shall use the naive approach using minors. Note that this is an
// O(n!) computation. There does exists a O(n^3.5 log n log log n) computation
// but it is theoretical and somewhat complicated. The issue with using the
// obvious row reduction method is that we cannot avoid division.
polynomial poly_matrix::determinant() const {
  if(size == 1) return (*this)[0][0];
  
  // We expand the determinant over the first row.
  polynomial acc(0.0);
  for(int col = 0; col != size; ++col) {
    polynomial cofactor((*this)[0][col]);
    if(col % 2 == 1) cofactor *= polynomial(-1);
    acc += cofactor * get_minor(0, col).determinant();
  }
  return acc;
}

polynomial poly_matrix::determinant() {
  // We accumulate the determinant with acc.
  polynomial acc(1.0);
  
  // The first step in RREF is to choose a (nonzero) pivot in the first
  // column if one exists.

  // The rowStart indicates where we should start searching for a pivot.
  // That is, we ignore all rows before rowStart (above it in the matrix)
  size_t rowStart = 0;

  // We apply this process of finding a pivot to each row,
  // although the rowStart may not increment each time (for instance
  // we don't find a pivot)
  for(size_t pivotColumn = 0; pivotColumn != size; ++pivotColumn) {
    // We find the first nonzero entry in this column. That entry becomes the
    // pivot.
    polynomial pivot(0.0);
    int pivotRow(rowStart);
    while(pivotRow != size) {
      if(!(*this)[pivotRow][pivotColumn].is_zero()) {
	pivot = (*this)[pivotRow][pivotColumn];
	break;
      }
      ++pivotRow;
    }
    // If we did not find a pivot, we move on to the next column.
    if(pivot.is_zero()) continue;

    if(pivotRow != rowStart) {
      acc *= polynomial(-1.0);
      // Swap the pivotRow and the rowStart
      for(int i = pivotColumn; i != size; ++i) {
	std::swap((*this)[pivotRow][i], (*this)[rowStart][i]);
      }
      
    }
    
    // Now for all rows below rowStart, we subtract the pivot
    // row multiplied by the first element in the examined row
    // from the product of the examined row and the pivot.
    //    acc /= pivot;
    for(int row = rowStart + 1; row != size; ++row) {
      for(int col = pivotColumn + 1; col != size; ++col) {
        (*this)[row][col] =
	  (*this)[row][col] * pivot -
	  (*this)[row][pivotColumn] * (*this)[rowStart][col];
      }
      (*this)[row][pivotColumn] = polynomial(0.0);
    }
    ++rowStart;
    if(rowStart == size) break;
  }
  // We are now upper triangular. We compute the determinant by multiplying
  // the diagonal entries.
  for(int i = 0; i != size; ++i) {
    acc *= (*this)[i][i];
  }
  return acc;
}
} // namespace math
