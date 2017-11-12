#ifndef MATRIX_GUARD
#define MATRIX_GUARD
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstring>
#include <cmath>

/*
* A generic class that represents a matrix. It is parameterized,
* number of rows, cols, and the type of number it stores (usually a double).
* These matrices are mutable for efficiency.
*
* NOTE: this is matrix may have it's size determined at run-time.
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
  template<class NUM_T>
  class matrix {
  protected:
    /*A ponter to an array which stores elements in ROW MAJOR order.
    That is, elements of the same row are contiguous in memory. It is
    thus always faster to traverse the matrix row by row.
    Thus, if r is the row and c is the column,
    arr[r*COLS + c] addresses the element.
    */
    NUM_T* arr;
    int ROWS, COLS;

    static constexpr double tolerance = 1.0e-10;
  public:
    /*We also need a destructor*/
    ~matrix() {
      delete[] arr;
    }

    /*Here, we implement a mov, copy, assignment/constructors*/
    /*Copy Constructor*/
    matrix(const matrix<NUM_T>& mat) : ROWS(mat.ROWS), COLS(mat.COLS) {
      arr = new NUM_T[ROWS*COLS];
      std::memcpy(arr, mat.arr, ROWS*COLS*sizeof(NUM_T));
    }

    /*Move Constructor*/
    matrix(matrix<NUM_T>&& mat) : ROWS(mat.ROWS), COLS(mat.COLS), arr(mat.arr) {
      /*Set to null so when it is deleted, it won't delete the memory we now own*/
      mat.arr = nullptr;
    }



    /*Copy Assignment*/
    matrix<NUM_T>& operator=(const matrix<NUM_T>& other) {
      /*Self assignment*/
      if(&other == this) return *this;
      delete[] arr;
      ROWS = other.ROWS;
      COLS = other.COLS;
      arr = new NUM_T[ROWS*COLS];
      std::memcpy(arr, other.arr, ROWS*COLS*sizeof(NUM_T));
      return *this;
    }

    matrix<NUM_T>& operator=(matrix<NUM_T>&& other) {
      if(&other == this) return *this;
      delete[] arr;
      ROWS = other.ROWS;
      COLS = other.COLS;
      arr = other.arr;
      other.arr = nullptr;
      return *this;
    }

    /*Used to initialize all elements of the matrix to a given value*/
    matrix(int rows, int cols, NUM_T val) : ROWS(rows), COLS(cols) {
      arr = new NUM_T[ROWS*COLS];
      for(int r = 0; r!=ROWS; ++r) {
        for(int c = 0; c != COLS; ++c) {
          (*this)[r][c] = val;
        }
      }
    }

    matrix(int rows, int cols) : ROWS(rows), COLS(cols) {
      arr = new NUM_T[rows*cols];
    }

    matrix() : matrix(0,0) { };


/*The following 3 constructors are used so that we can have a convenient variable argument list
The public constructor constructs a matrix by listing its elements in row major order
after specifying the row and column size.
That is, row size then column size then row 0 is listed, then row 1, then row 2, etc...*/
private:
    /*Used to prevent ambiguity in template pack constructor*/
    struct templatePackIntegerWrapper {
      int val, rows, cols;
      templatePackIntegerWrapper(int _val, int _rows, int _cols)
      : val(_val), rows(_rows), cols(_cols) { }
    };

    matrix(templatePackIntegerWrapper elementIndex, NUM_T elem)
    : ROWS(elementIndex.rows), COLS(elementIndex.cols) {
      assert(elementIndex.val == COLS*ROWS - 1);
      arr = new NUM_T[ROWS*COLS];
      (*this)[ROWS-1][COLS-1] = elem;
    }

    template<class... T>
    matrix(templatePackIntegerWrapper elementIndex,
      NUM_T elem, T... rest) :
      matrix(
        templatePackIntegerWrapper(elementIndex.val + 1, elementIndex.rows, elementIndex.cols)
        , rest...) {
      (*this)[elementIndex.val/COLS][elementIndex.val%COLS] = elem;
    }

public:
    template<class... T>
    matrix(int rows, int cols, NUM_T elem, T... rest) :
    matrix(templatePackIntegerWrapper(1, rows, cols), rest...) {
      (*this)[0][0] = elem;
    }

    int get_rows() const {
      return ROWS;
    }

    int get_cols() const {
      return COLS;
    }

    /*Returns the standard matrix norm
    which is the square root of the sum of the squres of all entries*/
    NUM_T norm() const {
      NUM_T sum = 0;
      for(int r = 0; r != ROWS; ++r) {
        for(int c = 0; c != COLS; ++c) {
          sum += arr[r][c]*arr[r][c];
        }
      }
      return std::sqrt(static_cast<double>(sum));
    }

    const NUM_T* data() const {
      return arr;
    }

    /*Returns a reference to the row array that may be edited freely*/
    NUM_T* operator[](int);

    /*Returns only a constant reference*/
    const NUM_T* operator[](int) const;

    /*Used for efficient addition, subtraction and scalar multiplication of matrices
    These operations mutate this matrix and return a reference to this*/
    /*NOTE: no checking is performed to ensure that the matrices have the same size,
    or in general are compatable.*/
    matrix<NUM_T>& operator+=(const matrix<NUM_T>&);
    matrix<NUM_T>& operator-=(const matrix<NUM_T>&);

    /*Note that this operation may be expensive since the
    resulting matrix will most likely require a resize.*/
    matrix<NUM_T>& operator*=(const matrix<NUM_T>&);
    matrix<NUM_T>& operator*=(NUM_T);
    matrix<NUM_T>& operator/=(NUM_T);
    matrix<NUM_T>& operator-();



    /*Here we implement some useful operations on matrices.
    Some operations are only defined on square matrices. NO CHECKING
    is done to ensure that this is the case, and the behaviour
    of those functions is undefined when called on matrices which are not square.
    */

    /*
    * Implements Row reduction into echelon form using the standard algorithm.
    * No performance optimizations are made yet. Returns a reference to itself.
    */
    matrix<NUM_T>& rref();

    /*
    * Computes the determinant of this matrix without altering it.
    */
    NUM_T determinant() const;

    /*
    * Iterates over the matrix, replacing every element within tolerance
    * of zero with the value +0.0. Sometimes it may be
    * useful to call this after various methods which do not guarentee that
    * values treated as zeros will be set to zero. Note that this function
    * mutates the matrix and returns a reference to itself.
    */
    matrix<NUM_T>& set_zeros();

    /*
    * Iterates over the matrix, replacing every element within tolerance
    * of one with the value 1.
    */
    matrix<NUM_T>& set_ones();

  private:
    matrix<NUM_T>& rref(NUM_T& determinant);
  };


  template<class NUM_T>
  matrix<NUM_T>& matrix<NUM_T>::set_zeros() {
    for(int row = 0; row != ROWS; ++row) {
      for(int col = 0; col != COLS; ++col) {
        if(std::abs(static_cast<double>((*this)[row][col])) < tolerance) {
          (*this)[row][col] = static_cast<NUM_T>(0);
        }
      }
    }
    return *this;
  }

  template<class NUM_T>
  matrix<NUM_T>& matrix<NUM_T>::set_ones() {
    for(int row = 0; row != ROWS; ++row) {
      for(int col = 0; col != COLS; ++col) {
        if(std::abs(static_cast<double>((*this)[row][col]) - 1.0) < tolerance)
          (*this)[row][col] = static_cast<NUM_T>(1);
      }
    }
    return *this;
  }

  template<class NUM_T>
  matrix<NUM_T>& matrix<NUM_T>::rref() {
    NUM_T det;
    rref(det);
    return *this;
  }

  template<class NUM_T>
  NUM_T matrix<NUM_T>::determinant() const {
    NUM_T det;
    matrix<NUM_T> mat(*this);
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
  template<class NUM_T>
  matrix<NUM_T>& matrix<NUM_T>::rref(NUM_T& determinant) {
    determinant = static_cast<NUM_T>(1);

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
    for(int pivotColumn = 0; pivotColumn != COLS; ++pivotColumn) {
      /*Holds the row index (not the value) of the largest pivot in std::absolute value in the column.
      It is initially set to rowStart*/
      int largestPivotIndex = rowStart;

      /*We traverse the column. We start at rowStart + 1 since the largestPivotIndex is already set to rowStart*/
      for(int pivotChoice = rowStart + 1; pivotChoice != ROWS; ++pivotChoice) {
        if(std::abs(static_cast<double>((*this)[largestPivotIndex][pivotColumn]))
         < std::abs(static_cast<double>((*this)[pivotChoice][pivotColumn])))
          largestPivotIndex = pivotChoice;
      }
      /*If after the traversal, largestPivotIndex points to a zero element, then the whole column is zero,
      so we may skip it and move on to the next column without incrementing rowStart.
      Since we are doing a comparison by zero,
      we will use the tolerance*/
      if(std::abs(static_cast<double>((*this)[largestPivotIndex][pivotColumn])) < tolerance) continue;


      /*Otherwise, We interchange it with the rowStart row. This is a potentially
      expensive operation (since we are using a 2d array and not an array of points) thus an adjacency
      list might be more efficient than a 2d array in this case*/
      NUM_T largestPivotValue = (*this)[largestPivotIndex][pivotColumn];
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
          NUM_T tmp = (*this)[rowStart][i];
          (*this)[rowStart][i] = (*this)[largestPivotIndex][i]/largestPivotValue;
          (*this)[largestPivotIndex][i] = tmp;

        }
      } else {
        /*In this case, we don't need to perform the
        swap, just the division*/
        for(int i = pivotColumn; i != COLS; ++i) {
          (*this)[rowStart][i] = (*this)[largestPivotIndex][i] / largestPivotValue;

        }
      }


      /*Now for all rows below rowStart, we subtract the pivot
       row multiplied by the first element in the examined row*/
      for(int examinedRow = rowStart + 1; examinedRow != ROWS; ++examinedRow) {
        NUM_T multiplier = (*this)[examinedRow][pivotColumn];
        /*This value is known to be zero*/
        (*this)[examinedRow][pivotColumn] = static_cast<NUM_T>(0);
        for(int examinedColumn = pivotColumn + 1; examinedColumn != COLS; ++examinedColumn) {
          (*this)[examinedRow][examinedColumn] -= multiplier*(*this)[rowStart][examinedColumn];
        }
      }

      /*Now, we can increment the rowStart*/
      ++rowStart;

      /*We are done with the first phase*/
      if(rowStart == ROWS) break;
    }
    if(rowStart != ROWS) {
      /*rowStart counts the number of pivots. If the matrix
      is square, and rowStart != ROWS, then it does not reduce to the identity,
      so the determinant is 0.
      */
      determinant = static_cast<NUM_T>(0);
    }

    /*In the second phase, we start with the last non zero row.
    We use its pivot (first nonzero value) to form zeros in the column above it*/

    for(rowStart = ROWS - 1; rowStart != -1; --rowStart) {
      for(int pivotColumn = 0; pivotColumn != COLS; ++pivotColumn) {
        if(std::abs(static_cast<double>((*this)[rowStart][pivotColumn])) >= tolerance) {
          /*We have found the first nonzero entry in the row*/
          /*We now subtract this row times a multiplier from all the above rows*/
          for(int examinedRow = 0; examinedRow != rowStart; ++examinedRow) {
            /*We can ignore previous values since they are zero*/
            NUM_T multiplier = (*this)[examinedRow][pivotColumn];
            /*We know that the examined element will become zero*/
            (*this)[examinedRow][pivotColumn] = static_cast<NUM_T>(0);
            for(int examinedColumn = pivotColumn+1; examinedColumn != COLS; ++examinedColumn) {
              (*this)[examinedRow][examinedColumn] -= multiplier*(*this)[rowStart][examinedColumn];
            }
          }
          break;
        }
      }
    }
    return *this;
  }

  template<class NUM_T>
  NUM_T* matrix<NUM_T>::operator[](int row) {
    return arr + COLS*row;
  }

  template<class NUM_T>
  const NUM_T* matrix<NUM_T>::operator[](int row) const {
    return arr + COLS*row;
  }

  template<class NUM_T>
  matrix<NUM_T>&
  matrix<NUM_T>::operator+=(const matrix<NUM_T>& mat) {
    assert(ROWS == mat.ROWS);
    assert(COLS == mat.COLS);
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        (*this)[r][c] += mat[r][c];
      }
    }
    return *this;
  }

  template<class NUM_T>
  matrix<NUM_T>&
  matrix<NUM_T>::operator-=(const matrix<NUM_T>& mat) {
    assert(ROWS == mat.ROWS);
    assert(COLS == mat.COLS);
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        (*this)[r][c] -= mat[r][c];
      }
    }
    return *this;
  }

  template<class NUM_T>
  matrix<NUM_T>&
  matrix<NUM_T>::operator*=(const matrix<NUM_T>& mat) {
    assert(COLS == mat.ROWS);
    NUM_T* newArr = new NUM_T[ROWS*mat.COLS];

    for(int row = 0; row != ROWS; ++row) {
      for(int col = 0; col != mat.COLS; ++col) {
        newArr[row*mat.COLS + col] = 0;
        for(int i = 0; i != COLS; ++i)
          newArr[row*mat.COLS + col] += (*this)[row][i]*mat[i][col];
      }
    }

    delete[] arr;
    arr = newArr;
    return *this;
  }

  template<class NUM_T>
  matrix<NUM_T>&
  matrix<NUM_T>::operator*=(NUM_T scal) {
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        (*this)[r][c] *= scal;
      }
    }
    return *this;
  }



  template<class NUM_T>
  matrix<NUM_T>&
  matrix<NUM_T>::operator/=(NUM_T scal) {
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        (*this)[r][c] /= scal;
      }
    }
    return *this;
  }

  template<class NUM_T>
  matrix<NUM_T>&
  matrix<NUM_T>::operator-() {
    return this *= -1;
  }



/*
* These are not member methods of the matrix class
* These are useful functions which perform operations on matrices
* and related classes but do not make sense as member functions
*/

  /*
  * Generates the appropriately sized square identity matrix
  */
  template<class NUM_T>
  matrix<NUM_T> gen_identity_matrix(int SIZE) {
    matrix<NUM_T> retMat(SIZE, SIZE);
    for(int r = 0; r != SIZE; ++r) {
      for(int c = 0; c != SIZE; ++c) {
        retMat[r][c] = (r == c)?static_cast<NUM_T>(1):static_cast<NUM_T>(0);
      }
    }
    return retMat;
  }


  /*
  * Adjoins the matrices by row. That is,
  * it the nth row of the returned matrix is the nth row of
  * the first matrix then the nth row of the second matrix.
  */
  template<class NUM_T>
  matrix<NUM_T>
  adjoin_by_row(const matrix<NUM_T>& mat1, const matrix<NUM_T>& mat2) {
    int ROWS = mat1.get_rows();
    int COLS1 = mat1.get_cols();
    int COLS2 = mat2.get_cols();
    assert(mat1.get_rows() == mat2.get_rows());
    matrix<NUM_T> retMat(ROWS, COLS1 + COLS2);
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
  * it the nth col of the returned matrix is the nth col of
  * the first matrix then the nth col of the second matrix.
  */
  template<class NUM_T>
  matrix<NUM_T>
  adjoin_by_column(const matrix<NUM_T>& mat1, const matrix<NUM_T>& mat2) {
    assert(mat1.get_cols() == mat2.get_cols());
    int COLS = mat1.get_cols();
    int ROWS1 = mat1.get_rows();
    int ROWS2 = mat2.get_rows();
    matrix<NUM_T> retMat(ROWS1 + ROWS2, COLS);
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



  template<class NUM_T>
  matrix<NUM_T>
  operator+(matrix<NUM_T> lhs, const matrix<NUM_T>& rhs) {
    return lhs += rhs;
  }

  template<class NUM_T>
  matrix<NUM_T>
  operator-(matrix<NUM_T> lhs, const matrix<NUM_T>& rhs) {
    return lhs -= rhs;
  }

  template<class NUM_T>
  matrix<NUM_T>
  operator*(matrix<NUM_T> lhs, const matrix<NUM_T>& rhs) {
    return lhs *= rhs;
  }

  template<class NUM_T>
  matrix<NUM_T> operator*(NUM_T scal, matrix<NUM_T> mat) {
    return mat *= scal;
  }

  template<class NUM_T>
  matrix<NUM_T> operator*(matrix<NUM_T> mat, NUM_T scal) {
    return mat *= scal;
  }

  template<class NUM_T>
  matrix<NUM_T> operator/(matrix<NUM_T> mat, NUM_T scal) {
    return mat /= scal;
  }



  template<class NUM_T>
  std::ostream& operator<<(std::ostream& out, const matrix<NUM_T>& mat) {
    int ROWS = mat.get_rows();
    int COLS = mat.get_cols();
    for(int row = 0; row != ROWS; ++row) {
      for(int col = 0; col != COLS; ++col) {
        out << mat[row][col];
        if(col != COLS - 1) out << "\t";
      }
      if(row != ROWS - 1) out << "\n";
    }
    return out;
  }

  template<class NUM_T>
  matrix<NUM_T> invert_matrix(const matrix<NUM_T>& mat) {
    assert(mat.get_cols() == mat.get_rows());
    int SIZE = mat.get_cols();
    matrix<NUM_T> augmented = adjoin_by_row(mat, gen_identity_matrix<NUM_T>(SIZE));
    //std::cout << augmented << std::endl;
    augmented.rref();
    //std::cout << augmented << std::endl;
    matrix<NUM_T> retMat(SIZE,SIZE);

    for(int row = 0; row != SIZE; ++row) {
      for(int col = 0; col != SIZE; ++col) {
        retMat[row][col] = augmented[row][col+SIZE];
      }
    }
    return retMat;
  }
}





#endif
