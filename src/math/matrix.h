#ifndef MATRIX_GUARD
#define MATRIX_GUARD

namespace math {
  template<class NUM_TYPE, int ROWS, int COLS>
  class matrix {
  public:
    matrix();
    NUM_TYPE& operator[](int);
    matrix<NUM_TYPE, ROWS, COLS>& operator+=(matrix<NUM_TYPE, ROWS, COLS> mat);
  private:
    NUM_TYPE arr[ROWS][COLS];
  };

  template<class NUM_TYPE, int ROWS, int COLS>
  matrix<NUM_TYPE, ROWS, COLS>::matrix()  {
    /*We always iterate in row major order. That is,
    elements of a row are stored contiguously so it is faster to access
    one row at a time*/
    for(int r = 0; r != ROWS; ++r) {
      for(int c = 0; c != COLS; ++c) {
        arr[r][c] = static_cast<NUM_TYPE>(0);
      }
    }
  }

  template<class NUM_TYPE, int ROWS, int COLS>
  NUM_TYPE& matrix<NUM_TYPE, ROWS, COLS>::operator[](int row, int col) {
    return arr[r][c];
  }



}


#endif
