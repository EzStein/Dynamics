#ifndef static_vector_GUARD
#define static_vector_GUARD
#include <iostream>

namespace math {
  template <class NUM_T, int SIZE>
  class static_vector {
  public:
    /*Default constructor which creates a static_vector of value 0*/
    static_vector();

    template<class OTHER_NUM_T>
    static_vector(const static_vector<OTHER_NUM_T, SIZE>& vec) {
      for(int i = 0; i != SIZE; ++i) {
        elements[i] = static_cast<NUM_T>(vec[i]);
      }
    }

    /*template<class... ARGS>
    static_vector(ARGS... args) {
      constructor_helper(args...);
    }*/



    NUM_T& operator[](int index);
    const NUM_T& operator[](int index) const;

    static_vector<NUM_T, SIZE>& operator+=(const static_vector<NUM_T, SIZE>&);
    static_vector<NUM_T, SIZE>& operator-=(const static_vector<NUM_T, SIZE>&);
    static_vector<NUM_T, SIZE>& operator*=(NUM_T);
    static_vector<NUM_T, SIZE>& operator/=(NUM_T);
    static_vector<NUM_T, SIZE>& operator-();
  private:
    NUM_T elements[SIZE];

    /*template<class... ARGS>
    void constructor_helper(int& count, NUM_T val1, ARGS... args) {
      elements[++count] = val1;
      constructor_helper(count, args...);
    }*/
  };

  template <class NUM_T, int SIZE>
  static_vector<NUM_T, SIZE>::static_vector() {
    for(int i = 0; i < SIZE; ++i) {
      elements[i] = static_cast<NUM_T>(0);
    }
  }



  template<class NUM_T, int SIZE>
  NUM_T& static_vector<NUM_T, SIZE>::operator[](int index) {
    return elements[index];
  }

  template<class NUM_T, int SIZE>
  const NUM_T& static_vector<NUM_T, SIZE>::operator[](int index) const {
    return elements[index];
  }

  template<class NUM_T, int SIZE>
  static_vector<NUM_T, SIZE>& static_vector<NUM_T, SIZE>::operator+=(const static_vector<NUM_T, SIZE>& val) {
    for(int i = 0; i < SIZE; ++i) {
      elements[i] += val.elements[i];
    }
    return *this;
  }

  template<class NUM_T, int SIZE>
  static_vector<NUM_T, SIZE>& static_vector<NUM_T, SIZE>::operator-=(const static_vector<NUM_T, SIZE>& val) {
    for(int i = 0; i < SIZE; ++i) {
      elements[i] -= val.elements[i];
    }
    return *this;
  }

  template<class NUM_T, int SIZE>
  static_vector<NUM_T, SIZE>& static_vector<NUM_T, SIZE>::operator*=(NUM_T val) {
    for(int i = 0; i < SIZE; ++i) {
      elements[i] *= val;
    }
    return *this;
  }

  template<class NUM_T, int SIZE>
  static_vector<NUM_T, SIZE>& static_vector<NUM_T, SIZE>::operator/=(NUM_T val) {
    for(int i = 0; i < SIZE; ++i) {
      elements[i] /= val;
    }
    return *this;
  }

  template<class NUM_T, int SIZE>
  static_vector<NUM_T, SIZE>& static_vector<NUM_T, SIZE>::operator-() {
    return *this *= -1;
  }

  /*What appears to be extra copies are made here, but these are removed with RVO and
  elision which actually makes this implementation faster*/

  template<class NUM_T, int SIZE>
  static_vector<NUM_T, SIZE> operator+(static_vector<NUM_T, SIZE> lhs, const static_vector<NUM_T, SIZE>& rhs) {
    return lhs += rhs;
  }

  template<class NUM_T, int SIZE>
  static_vector<NUM_T, SIZE> operator-(static_vector<NUM_T, SIZE> lhs, const static_vector<NUM_T, SIZE>& rhs) {
    return lhs -= rhs;
  }

  template<class NUM_T, int SIZE>
  static_vector<NUM_T, SIZE> operator*(NUM_T scal, static_vector<NUM_T, SIZE> vec) {
    return vec *= scal;
  }

  template<class NUM_T, int SIZE>
  static_vector<NUM_T, SIZE> operator*(static_vector<NUM_T, SIZE> vec, NUM_T scal) {
    return vec *= scal;
  }

  template<class NUM_T, int SIZE>
  static_vector<NUM_T, SIZE> operator/(static_vector<NUM_T, SIZE> vec, NUM_T scal) {
    return vec /= scal;
  }

  template<class NUM_T, int SIZE>
  std::ostream& operator<<(std::ostream& out, const static_vector<NUM_T, SIZE>& vec) {
    out << "(";
    for(int i = 0; i != SIZE - 1; ++i) {
      out << vec[i] << ", ";
    }
    out << vec[SIZE - 1] << ")";
    return out;
  }
}
#endif
