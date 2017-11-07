#ifndef static_matrix_GUARD
#define static_matrix_GUARD
#include <iostream>
#include <cassert>
#include <cmath>
#include "math/vector.h"

/*
* A generic class that represents a static_matrix. It is parameterized,
* number of rows, cols, and the type of number it stores (usually a double).
* These matrices are mutable for efficiency.

* NOTE: this static_matrix is static in the sense that it's size is determined at compile
* time. In this sense, it is slightly more efficient, then the general static_matrix
* class and has better type safety.
*/

/*
* NUM_T must have the following operations defined as NUM_T X NUM_T -> NUM_T
* + - * / *= += -= /=
*
* Additionally the following operators from NUM_T X NUM_T -> bool
* < <= > >= == !=
* and
* static_cast of integers to NUM_T must also be defined
* as well as static cast from NUM_T to double
*
* the << operator must be available for istream insertion
* NUM_T must have a default constructor if it is a class.
*/
namespace math {
  template<class NUM_T, int ROWS, int COLS>
  class static_matrix {
  private:
    /*An array which stores elements in ROW MAJOR order.
    That is, elements of the same row are contiguous in memory. It is
    thus always faster to traverse the static_matrix row by row.
    The first index indicates the row, and the second index indicates the column*/
    NUM_T arr[ROWS][COLS];

    static constexpr double tolerance = 1.0e-10;

    /*The type representing one row*/
    typedef NUM_T ROW_ARR_T[COLS];
  public:
    /*The standard copy/move constructor and copy/move assignment are used so
    there is no need to override them*/

    /*Used to initialize all elements of the static_matrix to a given value*/
    static_matrix(NUM_T val) {
      for(int r = 0; r!=ROWS; ++r) {
        for(int c = 0; c != COLS; ++c) {
          arr[r][c] = val;
        }
      }
    }

    static_matrix(int val = 0) {
      for(int r = 0; r!=ROWS; ++r) {
        for(int c = 0; c != COLS; ++c) {
          arr[r][c] = static_cast<NUM_T>(val);
        }
      }
    }

    template<class OTHER_NUM_T>
    static_matrix(const static_matrix<OTHER_NUM_T, ROWS, COLS>& mat) {
      for(int r = 0; r != ROWS; ++r) {
        for(int c = 0; c != COLS; ++c) {
          arr[r][c] = static_cast<NUM_T>(mat[r][c]);
        }
      }
    }




/*The following 3 constructors are used so that we can have a convenient variable argument list
The public constructor constructs a static_matrix by listing its elements in row major order.
That is, row 0 is listed, then row 1, then row 2, etc...*/
private:
    /*Used to prevent ambiguity in template pack constructor*/
    struct templatePackIntegerWrapper {
      int val;
      templatePackIntegerWrapper(int _val) : val(_val) { }
    };

    static_matrix(templatePackIntegerWrapper elementIndex, NUM_T elem) {
      assert(elementIndex.val == COLS*ROWS - 1);
      arr[ROWS-1][COLS-1] = elem;
    }

    template<class... T>
    static_matrix(templatePackIntegerWrapper elementIndex,
      NUM_T elem, T... rest) : static_matrix(templatePackIntegerWrapper(elementIndex.val + 1), rest...) {
      arr[elementIndex.val/COLS][elementIndex.val%COLS] = elem;
    }

public:
    template<class... T>
    static_matrix(NUM_T elem, T... rest) : static_matrix(templatePackIntegerWrapper(1), rest...) {
      arr[0][0] = elem;
    }

    /*Constructs this static_matrix from a column vector
    This constructor should only be used */
    static_matrix(const vector<NUM_T, ROWS>& vec) {
      assert(COLS = 1);
      for(int r = 0; r != ROWS; ++r) {
        arr[r][0] = vec[r];
      }
    }



    /*Returns a reference to the row array that may be edited freely*/
    ROW_ARR_T& operator[](int);
    /*Returns only a constant reference*/
    const ROW_ARR_T& operator[](int) const;

    /*Used for efficient addition, subtraction and scalar multiplication of matrices
    These operations mutate this static_matrix and return a reference to this*/
    static_matrix<NUM_T, ROWS, COLS>& operator+=(const static_matrix<NUM_T, ROWS, COLS>&);
    static_matrix<NUM_T, ROWS, COLS>& operator-=(const static_matrix<NUM_T, ROWS, COLS>&);
    static_matrix<NUM_T, ROWS, COLS>& operator*=(NUM_T);
    static_matrix<NUM_T, ROWS, COLS>& operator/=(NUM_T);
    static_matrix<NUM_T, ROWS, COLS>& operator-();



    /*Here we implement some useful operations on matrices.
    Some operations are only defined on square matrices. NO CHECKING
    is done to ensure that this is the case, and the behaviour
    of those functions is undefined when called on matrices which are not square.
    */

    /*
    * Implements Row reduction into echelon form using the standard algorithm.
    * No performance optimizations are made yet. Returns a reference to itself.
    */
    static_matrix<NUM_T, ROWS, COLS>& rref();

    /*
    * Computes the determinant of this static_matrix without altering it.
    */
    NUM_T determinant() const;
    /*
    * Iterates over the static_matrix, replacing every element within tolerance
    * of zero with the value +0.0. Sometimes it may be
    * useful to call this after various methods which do not guarentee that
    * values treated as zeros will be set to zero. Note that this function
    * mutates the static_matrix and returns a reference to itself.
    */
    static_matrix<NUM_T, ROWS, COLS>& set_zeros();

    /*
    * Iterates over the static_matrix, replacing every element within tolerance
    * of one with the value 1.
    */
    static_matrix<NUM_T, ROWS, COLS>& set_ones();

  private:
    static_matrix<NUM_T, ROWS, COLS>& rref(NUM_T& determinant);
  };


  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS>& static_matrix<NUM_T, ROWS, COLS>::set_zeros() {
    for(int row = 0; row != ROWS; ++row) {
      for(int col = 0; col != COLS; ++col) {
        if(std::abs(static_cast<double>(arr[row][col])) < tolerance) {
          arr[row][col] = static_cast<NUM_T>(0);
        }
      }
    }
    return *this;
  }

  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS>& static_matrix<NUM_T, ROWS, COLS>::set_ones() {
    for(int row = 0; row != ROWS; ++row) {
      for(int col = 0; col != COLS; ++col) {
        if(std::abs(static_cast<double>(arr[row][col]) - 1.0) < tolerance)
          arr[row][col] = static_cast<NUM_T>(1);
      }
    }
    return *this;
  }

  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS>& static_matrix<NUM_T, ROWS, COLS>::rref() {
    NUM_T det;
    rref(det);
    return *this;
  }

  template<class NUM_T, int ROWS, int COLS>
  NUM_T static_matrix<NUM_T, ROWS, COLS>::determinant() const {
    NUM_T det;
    static_matrix<NUM_T, ROWS, COLS> mat(*this);
    mat.rref(det);
    return det;
  }

  /*
  * This implementation treats zeros as follows:
  * Any number that is within tolerance of zero will be
  * treated as though it were a zero. However, for optimization
  * reasons, it will not be replaced with zero, but will be treated
  * as being equal to all values also within that tolerance.
  * Thus it may be useful to call, set_zeros() after calling
  * this method. The method will also calculate the determinant
  */
  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS>& static_matrix<NUM_T, ROWS, COLS>::rref(NUM_T& determinant) {
    determinant = static_cast<NUM_T>(1);

    /*The first step in RREF is to choose a (nonzero) pivot in the first column if one exists
    We will choose the largest pivot in std::absolute value in order to minimize instability*/

    /*The rowStart indicates where we should start searching for a pivot.
    That is, we ignore all rows before rowStart (above it in the static_matrix)*/
    int rowStart = 0;

    /*
    * We apply this process of finding a pivot to each row,
    * although the rowStart may not increment each time (for instance
      if we don't find a pivot)
    */
    for(int pivotColumn = 0; pivotColumn != COLS; ++pivotColumn) {
      /*Holds the row index (not the value) of the largest pivot in std::absolute value in the column.
      It is initially set to rowStart*/
      int largestPivotIndex = rowStart;

      /*We traverse the column. We start at rowStart + 1 since the largestPivotIndex is already set to rowStart*/
      for(int pivotChoice = rowStart + 1; pivotChoice != ROWS; ++pivotChoice) {
        if(std::abs(static_cast<double>(arr[largestPivotIndex][pivotColumn]))
         < std::abs(static_cast<double>(arr[pivotChoice][pivotColumn])))
          largestPivotIndex = pivotChoice;
      }
      /*If after the traversal, largestPivotIndex points to a zero element, then the whole column is zero,
      so we may skip it and move on to the next column without incrementing rowStart.
      Since we are doing a comparison by zero,
      we will use the tolerance*/
      if(std::abs(static_cast<double>(arr[largestPivotIndex][pivotColumn])) < tolerance) continue;


      /*Otherwise, We interchange it with the rowStart row. This is a potentially
      expensive operation (since we are using a 2d array and not an array of points) thus an adjacency
      list might be more efficient than a 2d array in this case*/
      NUM_T largestPivotValue = arr[largestPivotIndex][pivotColumn];
      /*In this case, we are multiplying the row by 1/largestPivotValue,
      so we multiply by the inverse (largestPivotValue) for the determinant*/
      determinant *= largestPivotValue;

      if(largestPivotIndex != rowStart) {
        /*If the largestPivotIndex is not already in the start row then:*/
        /*We can start with the pivot column because all values to the left should already be
        effectively zero*/
        /*We multiply the determinant by -1 for the swap*/
        determinant *= static_cast<NUM_T>(-1);
        for(int i = pivotColumn; i != COLS; ++i) {
          /*Performs the swap and division*/
          NUM_T tmp = arr[rowStart][i];
          arr[rowStart][i] = arr[largestPivotIndex][i]/largestPivotValue;
          arr[largestPivotIndex][i] = tmp;

        }
      } else {
        /*In this case, we don't need to perform the
        swap, just the division*/
        for(int i = pivotColumn; i != COLS; ++i) {
          arr[rowStart][i] = arr[largestPivotIndex][i] / largestPivotValue;

        }
      }


      /*Now for all rows below rowStart, we subtract the pivot
       row multiplied by the first element in the examined row*/
      for(int examinedRow = rowStart + 1; examinedRow != ROWS; ++examinedRow) {
        NUM_T multiplier = arr[examinedRow][pivotColumn];
        /*This value is known to be zero*/
        arr[examinedRow][pivotColumn] = static_cast<NUM_T>(0);
        for(int examinedColumn = pivotColumn + 1; examinedColumn != COLS; ++examinedColumn) {
          arr[examinedRow][examinedColumn] -= multiplier*arr[rowStart][examinedColumn];
        }
      }

      /*Now, we can increment the rowStart*/
      ++rowStart;

      /*We are done with the first phase*/
      if(rowStart == ROWS) break;
    }
    if(rowStart != ROWS) {
      /*rowStart counts the number of pivots. If the static_matrix
      is square, and rowStart != ROWS, then it does not reduce to the identity,
      so the determinant is 0.
      */
      determinant = static_cast<NUM_T>(0);
    }

    /*In the second phase, we start with the last non zero row.
    We use its pivot (first nonzero value) to form zeros in the column above it*/

    for(rowStart = ROWS - 1; rowStart != -1; --rowStart) {
      for(int pivotColumn = 0; pivotColumn != COLS; ++pivotColumn) {
        if(std::abs(static_cast<double>(arr[rowStart][pivotColumn])) >= tolerance) {
          /*We have found the first nonzero entry in the row*/
          /*We now subtract this row times a multiplier from all the above rows*/
          for(int examinedRow = 0; examinedRow != rowStart; ++examinedRow) {
            /*We can ignore previous values since they are zero*/
            NUM_T multiplier = arr[examinedRow][pivotColumn];
            /*We know that the examined element will become zero*/
            arr[examinedRow][pivotColumn] = static_cast<NUM_T>(0);
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
  typename static_matrix<NUM_T, ROWS, COLS>::ROW_ARR_T& static_matrix<NUM_T, ROWS, COLS>::operator[](int row) {
    return arr[row];
  }

  template<class NUM_T, int ROWS, int COLS>
  const typename static_matrix<NUM_T, ROWS, COLS>::ROW_ARR_T& static_matrix<NUM_T, ROWS, COLS>::operator[](int row) const {
    return arr[row];
  }

  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS>&
  static_matrix<NUM_T, ROWS, COLS>::operator+=(const static_matrix<NUM_T, ROWS, COLS>& mat) {
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        arr[r][c] += mat.arr[r][c];
      }
    }
    return *this;
  }

  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS>&
  static_matrix<NUM_T, ROWS, COLS>::operator-=(const static_matrix<NUM_T, ROWS, COLS>& mat) {
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        arr[r][c] -= mat.arr[r][c];
      }
    }
    return *this;
  }

  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS>&
  static_matrix<NUM_T, ROWS, COLS>::operator*=(NUM_T scal) {
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        arr[r][c] *= scal;
      }
    }
    return *this;
  }

  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS>&
  static_matrix<NUM_T, ROWS, COLS>::operator/=(NUM_T scal) {
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        arr[r][c] /= scal;
      }
    }
    return *this;
  }

  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS>&
  static_matrix<NUM_T, ROWS, COLS>::operator-() {
    return this *= -1;
  }



/*
* These are not member methods of the static_matrix class
* These are useful functions which perform operations on matrices
* and related classes but do not make sense as member functions
*/

  /*
  * Generates the appropriately sized square identity static_matrix
  */
  template<class NUM_T, int SIZE>
  static_matrix<NUM_T, SIZE, SIZE> gen_identity() {
    static_matrix<NUM_T, SIZE, SIZE> retMat;
    for(int r = 0; r != SIZE; ++r) {
      for(int c = 0; c != SIZE; ++c) {
        retMat[r][c] = (r == c)?static_cast<NUM_T>(1):static_cast<NUM_T>(0);
      }
    }
    return retMat;
  }


  /*
  * Adjoins the matrices by row. That is,
  * it the nth row of the returned static_matrix is the nth row of
  * the first static_matrix then the nth row of the second static_matrix.
  */
  template<class NUM_T, int ROWS, int COLS1, int COLS2>
  static_matrix<NUM_T, ROWS, COLS1 + COLS2>
  adjoin_by_row(const static_matrix<NUM_T, ROWS, COLS1>& mat1, const static_matrix<NUM_T, ROWS, COLS2>& mat2) {
    static_matrix<NUM_T, ROWS, COLS1 + COLS2> retMat;
    for(int row = 0; row != ROWS; ++row) {
      for(int col = 0; col != COLS1 + COLS2; ++col) {
        if(col < COLS1)
          retMat[row][col] = mat1[row][col];
        else
          retMat[row][col] = mat2[row][col-COLS1];
      }
    }
    return retMat;
  }

  /*
  * Adjoins the matrices by column. That is,
  * it the nth col of the returned static_matrix is the nth col of
  * the first static_matrix then the nth col of the second static_matrix.
  */
  template<class NUM_T, int ROWS1, int ROWS2, int COLS>
  static_matrix<NUM_T, ROWS1 + ROWS2, COLS>
  adjoin_by_column(const static_matrix<NUM_T, ROWS1, COLS>& mat1, const static_matrix<NUM_T, ROWS2, COLS>& mat2) {
    static_matrix<NUM_T, ROWS1+ROWS2, COLS> retMat;
    for(int row = 0; row != ROWS1 + ROWS2; ++row) {
      for(int col = 0; col != COLS; ++col) {
        if(row < ROWS1)
          retMat[row][col] = mat1[row][col];
        else
          retMat[row][col] = mat2[row - ROWS1][col];
      }
    }
    return retMat;
  }



  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS>
  operator+(static_matrix<NUM_T, ROWS, COLS> lhs, const static_matrix<NUM_T, ROWS, COLS>& rhs) {
    return lhs += rhs;
  }

  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS>
  operator-(static_matrix<NUM_T, ROWS, COLS> lhs, const static_matrix<NUM_T, ROWS, COLS>& rhs) {
    return lhs -= rhs;
  }

  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS> operator*(NUM_T scal, static_matrix<NUM_T, ROWS, COLS> mat) {
    return mat *= scal;
  }

  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS> operator*(static_matrix<NUM_T, ROWS, COLS> mat, NUM_T scal) {
    return mat *= scal;
  }

  template<class NUM_T, int ROWS, int COLS>
  static_matrix<NUM_T, ROWS, COLS> operator/(static_matrix<NUM_T, ROWS, COLS> mat, NUM_T scal) {
    return mat /= scal;
  }

  template<class NUM_T, int ROWS1, int COLS1, int COLS2>
  static_matrix<NUM_T, ROWS1, COLS2>
  operator*(const static_matrix<NUM_T, ROWS1, COLS1>& mat1, const static_matrix<NUM_T, COLS1, COLS2>& mat2) {
    static_matrix<NUM_T, ROWS1, COLS2> retMat(0);
    for(int row = 0; row != ROWS1; ++row) {
      for(int col = 0; col != COLS2; ++col) {
        for(int i = 0; i != COLS1; ++i)
          retMat[row][col] += mat1[row][i]*mat2[i][col];
      }
    }
    return retMat;
  }

  template<class NUM_T, int ROWS, int COLS>
  vector<NUM_T, ROWS> operator*(const static_matrix<NUM_T, ROWS, COLS>& mat, const vector<NUM_T, COLS>& vec) {
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
  std::ostream& operator<<(std::ostream& out, const static_matrix<NUM_T, ROWS, COLS>& mat) {
    for(int row = 0; row != ROWS; ++row) {
      for(int col = 0; col != COLS; ++col) {
        out << mat[row][col];
        if(col != COLS - 1) out << "\t";
      }
      if(row != ROWS - 1) out << "\n";
    }
    return out;
  }

  template<class NUM_T, int SIZE>
  static_matrix<NUM_T, SIZE, SIZE> invert(const static_matrix<NUM_T, SIZE, SIZE>& mat) {
    static_matrix<NUM_T, SIZE, 2*SIZE> augmented = adjoin_by_row(mat, gen_identity<NUM_T, SIZE>());
    //std::cout << augmented << std::endl;
    augmented.rref();
    //std::cout << augmented << std::endl;
    static_matrix<NUM_T, SIZE, SIZE> retMat;

    for(int row = 0; row != SIZE; ++row) {
      for(int col = 0; col != SIZE; ++col) {
        retMat[row][col] = augmented[row][col+SIZE];
      }
    }

    return retMat;
  }
}





#endif
