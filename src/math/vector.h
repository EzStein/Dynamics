#ifndef VECTOR_GUARD
#define VECTOR_GUARD
#include "math/matrix.h"
namespace math {
  /*Represents a column vector. This is just a matrix with COLS 1 and ROWS the size of the matrix*/
  template<class NUM_T>
  class vector : public matrix<NUM_T> {
  private:
  public:
    vector() : matrix<NUM_T>() { }

    vector(int size) : matrix<NUM_T>(size, 1) { }

    vector(int size, NUM_T val) : matrix<NUM_T>(size, 1, val) { }

    /*Default copy/move constructor*/
    vector(const vector<NUM_T>& vec) : matrix<NUM_T>(vec) { }
    vector(vector<NUM_T>&& vec) : matrix<NUM_T>(vec) { }

    /*Assignment operators are not inherited!*/
    vector<NUM_T>& operator=(const vector<NUM_T>& vec) {
      matrix<NUM_T>::operator=(vec);
      return *this;
    }

    vector<NUM_T>& operator=(vector<NUM_T>&& vec) {
      matrix<NUM_T>::operator=(vec);
      return *this;
    }

    NUM_T& operator[](int index);
    const NUM_T& operator[](int index) const;
    int size() const;
  };

  template<class NUM_T>
  int vector<NUM_T>::size() const {
    return this->ROWS;
  }

  template<class NUM_T>
  NUM_T& vector<NUM_T>::operator[](int index) {
    return matrix<NUM_T>::operator[](index)[0];
  }


  template<class NUM_T>
  const NUM_T& vector<NUM_T>::operator[](int index) const {
    return matrix<NUM_T>::operator[](index)[0];
  }

  /*Solves the system of equations,
  mat * sol = vec
  and returns the result sol as a parameter. If the solution does not exist or is not unique
  (if mat is not invertible) then the function returns false. Otherwise it returns true
  if a unique solution was found.
  NOTE: the matrix should be square with size the same as vec and sol
  */
  template<class NUM_T>
  bool solve_uniquely(const matrix<NUM_T>& mat, const math::vector<NUM_T>& vec, vector<NUM_T>& sol) {
    assert(mat.get_cols() == mat.get_rows());
    assert(vec.size() <= sol.size());
    assert(vec.size() == mat.get_cols());
    matrix<NUM_T> augmented = adjoin_by_row(mat, vec).rref().set_ones().set_zeros();
    /*We now check that the first part of the matrix is the identity*/
    for(int r = 0; r != augmented.get_rows(); ++r) {
      for(int c = 0; c != augmented.get_cols() - 1; ++c) {
        if(r == c) {
          if(augmented[r][c] != static_cast<NUM_T>(1))
            return false;
        } else {
          if(augmented[r][c] != static_cast<NUM_T>(0))
            return false;
        }
      }
    }

    for(int i = 0; i != augmented.get_rows(); ++i) {
      sol[i] = augmented[i][augmented.get_cols() - 1];
    }
    return true;
  }
}

#endif
