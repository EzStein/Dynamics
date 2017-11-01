#ifndef MATRIX_GUARD
#define MATRIX_GUARD
#include <iostream>
#include <cassert>
#include "math/vector.h"

/*
* A generic class that represents a matrix. It is parameterized,
* number of rows, cols, and the type of number it stores (usually a double).
* These matrices are mutable for efficiency.
*/
namespace math {
  template<class NUM_T, int ROWS, int COLS>
  class matrix {
  private:
    /*An array which stores elements in ROW MAJOR order.
    That is, elements of the same row are contiguous in memory. It is
    thus always faster to traverse the matrix row by row.
    The first index indicates the row, and the second index indicates the column*/
    NUM_T arr[ROWS][COLS];

    /*The type representing one row*/
    typedef NUM_T ROW_ARR_T[COLS];
  public:
    /*The standard copy/move constructor and copy/move assignment are used so
    there is no need to override them*/

    /*Used to initialize all elements of the matrix to a given value*/
    matrix(NUM_T val = 0) {
      for(int r = 0; r!=ROWS; ++r) {
        for(int c = 0; c != COLS; ++c) {
          arr[r][c] = val;
        }
      }
    }

    template<class OTHER_NUM_T>
    matrix(const matrix<OTHER_NUM_T, ROWS, COLS>& mat) {
      for(int r = 0; r != ROWS; ++r) {
        for(int c = 0; c != COLS; ++c) {
          arr[r][c] = static_cast<NUM_T>(mat[r][c]);
        }
      }
    }


/*The following 3 constructors are used so that we can have a convenient variable argument list
The public constructor constructs a matrix by listing its elements in row major order.
That is, row 0 is listed, then row 1, then row 2, etc...*/
private:
    /*Used to prevent ambiguity in template pack constructor*/
    struct templatePackIntegerWrapper {
      int val;
      templatePackIntegerWrapper(int _val) : val(_val) { }
    };

    matrix(templatePackIntegerWrapper elementIndex, NUM_T elem) {
      assert(elementIndex.val == COLS*ROWS - 1);
      arr[ROWS-1][COLS-1] = elem;
    }

    template<class... T>
    matrix(templatePackIntegerWrapper elementIndex,
      NUM_T elem, T... rest) : matrix(templatePackIntegerWrapper(elementIndex.val + 1), rest...) {
      arr[elementIndex.val/COLS][elementIndex.val%COLS] = elem;
    }

public:
    template<class... T>
    matrix(NUM_T elem, T... rest) : matrix(templatePackIntegerWrapper(1), rest...) {
      arr[0][0] = elem;
    }



    /*Returns a reference to the row array that may be edited freely*/
    ROW_ARR_T& operator[](int);
    /*Returns only a constant reference*/
    const ROW_ARR_T& operator[](int) const;

    /*Used for efficient addition, subtraction and scalar multiplication of matrices
    These operations mutate this matrix and return a reference to this*/
    matrix<NUM_T, ROWS, COLS>& operator+=(const matrix<NUM_T, ROWS, COLS>&);
    matrix<NUM_T, ROWS, COLS>& operator-=(const matrix<NUM_T, ROWS, COLS>&);
    matrix<NUM_T, ROWS, COLS>& operator*=(NUM_T);
    matrix<NUM_T, ROWS, COLS>& operator/=(NUM_T);
    matrix<NUM_T, ROWS, COLS>& operator-();

    /*Here we implement some useful operations on matrices.
    Some operations are only defined on square matrices. NO CHECKING
    is done to ensure that this is the case, and the behaviour
    of those functions is undefined when called on matrices which are not square.
    */

    /*
    * Implements Row reduction into echelon form using the standard algorithm.
    * No performance optimizations are made yet. Returns a reference to itself.
    */
    matrix<NUM_T, ROWS, COLS>& rref();
    //NUM_T determinant() const;
  };

  template<class NUM_T, int ROWS, int COLS>
  matrix<NUM_T, ROWS, COLS>& matrix<NUM_T, ROWS, COLS>::rref() {
    /*The first step in RREF is to choose a (nonzero) pivot in the first column if one exists
    We will choose the largest pivot in absolute value in order to minimize instability*/

    /*The rowStart indicates where we should start searching for a pivot.
    That is, we ignore all rows below rowStart*/
    int rowStart = 0;

    for(int pivotColumn = 0; pivotColumn != COLS; ++pivotColumn) {
      /*Holds the index (not the value) of the largest pivot in absolute value in the column.
      It is initially set to rowStart*/
      int largestPivotIndex = rowStart;

      /*We traverse the column. We start at rowStart + 1 since the largestPivotIndex is already set to rowStart*/
      for(int pivotChoice = rowStart + 1; pivotChoice != ROWS; ++pivotChoice) {
        if(abs(arr[largestPivotIndex][pivotColumn]) < abs(arr[pivotChoice][pivotColumn]))
          largestPivotIndex = pivotChoice;
      }
      /*If after the traversal, largestPivotIndex points to a zero element, then the whole column is zero,
      so we may skip it*/
      if(arr[largestPivotIndex][pivotColumn] == 0) continue;



      /*Otherwise, We interchange it with the rowStart row. This is a potentially
      expensive operation (since we are using a 2d array and not an array of points) thus an adjacency
      list might be more efficient than a 2d array in this case*/
      /*
      * At the same time, we can divide the moved row through by the pivot,
      * this will allow us to do only one traversal.
      */
      int largestPivotValue = arr[largestPivotIndex][pivotColumn];

      /*We can start with the pivot column because all values to the left should already be zero*/
      for(int i = pivotColumn; i != COLS; ++i) {
        /*Performs the swap and division*/
        NUM_T tmp = arr[rowStart][i];
        arr[rowStart][i] = arr[largestPivotIndex][i]/largestPivotValue;
        arr[largestPivotIndex][i] = tmp;
      }

      /*Now for all rows below rowStart, we subtract the pivot
       row multiplied by the first element in the examined row*/
      for(int examinedRow = rowStart + 1; examinedRow != ROWS; ++examinedRow) {
        NUM_T multiplier = arr[examinedRow][pivotColumn];
        arr[examinedRow][pivotColumn] = 0;
        for(int examinedColumn = pivotColumn + 1; examinedColumn != COLS; ++examinedColumn) {
          arr[examinedRow][examinedColumn] -= multiplier*arr[rowStart][examinedColumn];
        }
      }

      /*Now, we can increment the rowStart*/
      ++rowStart;
      /*We are done with the first phase*/
      if(rowStart == ROWS) break;
    }

    /*In the second phase, we start with the last non zero row.
    We use its pivot (first nonzero value) to form zeros in the column above it*/

    for(rowStart = ROWS - 1; rowStart != -1; --rowStart) {
      for(int pivotColumn = 0; pivotColumn != COLS; ++pivotColumn) {
        if(arr[rowStart][pivotColumn] != 0) {
          /*We have found the first nonzero entry in the row*/
          /*We now subtract this row times a multiplier from all the above rows*/
          for(int examinedRow = 0; examinedRow != rowStart; ++examinedRow) {
            /*We can ignore previous values since they are zero*/
            NUM_T multiplier = arr[examinedRow][pivotColumn];
            /*We know that the examined element will become zero*/
            arr[examinedRow][pivotColumn] = 0;
            for(int examinedColumn = pivotColumn+1; examinedColumn != COLS; ++examinedColumn) {
              arr[examinedRow][examinedColumn] -= multiplier*arr[rowStart][examinedColumn];
            }
          }
          break;
        }
      }
    }
    return *this;
  }

  template<class NUM_T, int ROWS, int COLS>
  typename matrix<NUM_T, ROWS, COLS>::ROW_ARR_T& matrix<NUM_T, ROWS, COLS>::operator[](int row) {
    return arr[row];
  }

  template<class NUM_T, int ROWS, int COLS>
  const typename matrix<NUM_T, ROWS, COLS>::ROW_ARR_T& matrix<NUM_T, ROWS, COLS>::operator[](int row) const {
    return arr[row];
  }

  template<class NUM_T, int ROWS, int COLS>
  matrix<NUM_T, ROWS, COLS>&
  matrix<NUM_T, ROWS, COLS>::operator+=(const matrix<NUM_T, ROWS, COLS>& mat) {
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        arr[r][c] += mat.arr[r][c];
      }
    }
    return *this;
  }

  template<class NUM_T, int ROWS, int COLS>
  matrix<NUM_T, ROWS, COLS>&
  matrix<NUM_T, ROWS, COLS>::operator-=(const matrix<NUM_T, ROWS, COLS>& mat) {
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        arr[r][c] -= mat.arr[r][c];
      }
    }
    return *this;
  }

  template<class NUM_T, int ROWS, int COLS>
  matrix<NUM_T, ROWS, COLS>&
  matrix<NUM_T, ROWS, COLS>::operator*=(NUM_T scal) {
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        arr[r][c] *= scal;
      }
    }
    return *this;
  }

  template<class NUM_T, int ROWS, int COLS>
  matrix<NUM_T, ROWS, COLS>&
  matrix<NUM_T, ROWS, COLS>::operator/=(NUM_T scal) {
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        arr[r][c] /= scal;
      }
    }
    return *this;
  }

  template<class NUM_T, int ROWS, int COLS>
  matrix<NUM_T, ROWS, COLS>&
  matrix<NUM_T, ROWS, COLS>::operator-() {
    return this *= -1;
  }



/*
* These are not member methods of the matrix class
*/
  template<class NUM_T, int ROWS, int COLS>
  matrix<NUM_T, ROWS, COLS>
  operator+(matrix<NUM_T, ROWS, COLS> rhs, const matrix<NUM_T, ROWS, COLS>& lhs) {
    return rhs += lhs;
  }

  template<class NUM_T, int ROWS, int COLS>
  matrix<NUM_T, ROWS, COLS>
  operator-(matrix<NUM_T, ROWS, COLS> rhs, const matrix<NUM_T, ROWS, COLS>& lhs) {
    return rhs -= lhs;
  }

  template<class NUM_T, int ROWS, int COLS>
  matrix<NUM_T, ROWS, COLS> operator*(NUM_T scal, matrix<NUM_T, ROWS, COLS> mat) {
    return mat *= scal;
  }

  template<class NUM_T, int ROWS, int COLS>
  matrix<NUM_T, ROWS, COLS> operator*(matrix<NUM_T, ROWS, COLS> mat, NUM_T scal) {
    return mat *= scal;
  }

  template<class NUM_T, int ROWS, int COLS>
  matrix<NUM_T, ROWS, COLS> operator/(matrix<NUM_T, ROWS, COLS> mat, NUM_T scal) {
    return mat /= scal;
  }

  template<class NUM_T, int ROWS, int COLS>
  vector<NUM_T, ROWS> operator*(const matrix<NUM_T, ROWS, COLS>& mat, const vector<NUM_T, COLS>& vec) {
    vector<NUM_T, ROWS> ret;
    for(int r = 0; r != ROWS; ++r) {
      NUM_T acc = static_cast<NUM_T>(0);
      for(int c = 0; c != COLS; ++c) {
        acc += mat[r][c] * vec[c];
      }
      ret[r] = acc;
    }
    return ret;
  }

  template<class NUM_T, int ROWS, int COLS>
  std::ostream& operator<<(std::ostream& out, const matrix<NUM_T, ROWS, COLS>& mat) {
    for(int row = 0; row != ROWS; ++row) {
      for(int col = 0; col != COLS; ++col) {
        out << mat[row][col];
        if(col != COLS - 1) out << "\t";
      }
      if(row != ROWS - 1) out << "\n";
    }
    return out;
  }
}




#endif
