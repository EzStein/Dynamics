#ifndef MATRIX_GUARD
#define MATRIX_GUARD

namespace math {
  template<class NUM_T, int ROWS, int COLS>
  class matrix {
  private:
    NUM_T arr[ROWS][COLS];
    typedef NUM_T ROW_ARR_T[COLS];
  public:
    matrix();

    template<class OTHER_NUM_T>
    matrix(const matrix<OTHER_NUM_T, ROWS, COLS>& mat) {
      for(int r = 0; r != ROWS; ++r) {
        for(int c = 0; c != COLS; ++c) {
          arr[r][c] = static_cast<NUM_T>(mat[r][c]);
        }
      }
    }

    /*Returns a reference to the row array*/
    ROW_ARR_T& operator[](int);
    const ROW_ARR_T& operator[](int) const;

    matrix<NUM_T, ROWS, COLS>& operator+=(const matrix<NUM_T, ROWS, COLS>&);
    matrix<NUM_T, ROWS, COLS>& operator-=(const matrix<NUM_T, ROWS, COLS>&);
    matrix<NUM_T, ROWS, COLS>& operator*=(NUM_T);
    matrix<NUM_T, ROWS, COLS>& operator/=(NUM_T);
    matrix<NUM_T, ROWS, COLS>& operator-();

  };

  template<class NUM_T, int ROWS, int COLS>
  matrix<NUM_T, ROWS, COLS>::matrix()  {
    /*We always iterate in row major order. That is,
    elements of a row are stored contiguously so it is faster to access
    one row at a time*/
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        arr[r][c] = static_cast<NUM_T>(0);
      }
    }
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
}


#endif
