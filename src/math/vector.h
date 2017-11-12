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

}

#endif
