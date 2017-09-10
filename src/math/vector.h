#ifndef VECTOR_GUARD
#define VECTOR_GUARD

namespace math {
  template <class NUM_T, int SIZE>
  class vector {
  public:
    /*Default constructor which creates a vector of value 0*/
    vector();

    NUM_T& operator[](int index);
    const NUM_T& operator[](int index) const;

    vector<NUM_T, SIZE>& operator+=(const vector<NUM_T, SIZE>&);
    vector<NUM_T, SIZE>& operator-=(const vector<NUM_T, SIZE>&);
    vector<NUM_T, SIZE>& operator*=(NUM_T);
    vector<NUM_T, SIZE>& operator/=(NUM_T);
    vector<NUM_T, SIZE>& operator-();
  private:
    NUM_T elements[SIZE];
  };

  template <class NUM_T, int SIZE>
  vector<NUM_T, SIZE>::vector() {
    for(int i = 0; i < SIZE; ++i) {
      elements[i] = static_cast<NUM_T>(0);
    }
  }

  template<class NUM_T, int SIZE>
  NUM_T& vector<NUM_T, SIZE>::operator[](int index) {
    return elements[index];
  }

  template<class NUM_T, int SIZE>
  const NUM_T& vector<NUM_T, SIZE>::operator[](int index) const {
    return elements[index];
  }

  template<class NUM_T, int SIZE>
  vector<NUM_T, SIZE>& vector<NUM_T, SIZE>::operator+=(const vector<NUM_T, SIZE>& val) {
    for(int i = 0; i < SIZE; ++i) {
      elements[i] += val.elements[i];
    }
    return *this;
  }

  template<class NUM_T, int SIZE>
  vector<NUM_T, SIZE>& vector<NUM_T, SIZE>::operator-=(const vector<NUM_T, SIZE>& val) {
    for(int i = 0; i < SIZE; ++i) {
      elements[i] -= val.elements[i];
    }
    return *this;
  }

  template<class NUM_T, int SIZE>
  vector<NUM_T, SIZE>& vector<NUM_T, SIZE>::operator*=(NUM_T val) {
    for(int i = 0; i < SIZE; ++i) {
      elements[i] *= val;
    }
    return *this;
  }

  template<class NUM_T, int SIZE>
  vector<NUM_T, SIZE>& vector<NUM_T, SIZE>::operator/=(NUM_T val) {
    for(int i = 0; i < SIZE; ++i) {
      elements[i] /= val;
    }
    return *this;
  }

  template<class NUM_T, int SIZE>
  vector<NUM_T, SIZE>& vector<NUM_T, SIZE>::operator-() {
    return *this *= -1;
  }

  /*What appears to be extra copies are made here, but these are removed with RVO and
  elision which actually makes this implementation faster*/

  template<class NUM_T, int SIZE>
  vector<NUM_T, SIZE> operator+(vector<NUM_T, SIZE> lhs, const vector<NUM_T, SIZE>& rhs) {
    return lhs += rhs;
  }

  template<class NUM_T, int SIZE>
  vector<NUM_T, SIZE> operator-(vector<NUM_T, SIZE> lhs, const vector<NUM_T, SIZE>& rhs) {
    return lhs -= rhs;
  }

  template<class NUM_T, int SIZE>
  std::ostream& operator<<(std::ostream& out, const vector<NUM_T, SIZE>& vec) {
    out << "(";
    for(int i = 0; i != SIZE - 1; ++i) {
      out << vec[i] << ", ";
    }
    out << vec[SIZE - 1] << ")";
    return out;
  }
}
#endif
