#ifndef VECTOR_2D_GUARD
#define VECTOR_2D_GUARD
#include <string>
#include <sstream>

//Represents a vector of two elements x and y whose type is the template numeric type.
template <class NUM_TYPE>
class vector_2d {
public:
  const NUM_TYPE x;
  const NUM_TYPE y;
  vector_2d(NUM_TYPE _x, NUM_TYPE _y) : x(_x), y(_y) { };
  template <class A> vector_2d(const vector_2d<A>& vec)
  : x(static_cast<NUM_TYPE>(vec.x)), y(static_cast<NUM_TYPE>(vec.y)) { };
  operator std::string() const;
};

template<class A, class B> vector_2d<A> operator+(vector_2d<A> vec1, vector_2d<B> vec2);
template<class A, class B> vector_2d<A> operator-(vector_2d<A> vec1, vector_2d<B> vec2);
template<class A, class B> vector_2d<B> operator*(vector_2d<B> vec, A scalar);
template<class A, class B> vector_2d<B> operator*(A scalar, vector_2d<B> vec);



template<class NUM_TYPE> vector_2d<NUM_TYPE>::operator std::string() const {
  std::ostringstream sstream;
  sstream << '(' << x << ',' << y << ')';
  return sstream.str();
}

/*
* NOTE: When the type of both inputs is the same, this function will behave as expected.
* However if the types differ, the resultant type will be the same as that of the
* first operand! Thus be wary about adding ints to doubles!
*/
template<class A, class B> vector_2d<A> operator+(vector_2d<A> vec1, vector_2d<B> vec2) {
  return vector_2d<A>(vec1.x + vec2.x, vec1.y + vec2.y);
}

/*
* NOTE: When the type of both inputs is the same, this function will behave as expected.
* However if the types differ, the resultant type will be the same as that of the
* first operand! Thus be wary about subtracting doubles from ints!
*/
template<class A, class B> vector_2d<A> operator-(vector_2d<A> vec1, vector_2d<B> vec2) {
  return vector_2d<A>(vec1.x - vec2.x, vec1.y - vec2.y);
}

/*
* Note the resultant type will be a vector_2d of the same type as the input operand.
*/
template<class A, class B> vector_2d<B> operator*(A scalar, vector_2d<B> vec) {
  return vector_2d<B>(scalar * vec.x, scalar * vec.y);
}

/*
* Note the resultant type will be a vector_2d of the same type as the input operand.
*/
template<class A, class B> vector_2d<B> operator*(vector_2d<B> vec, A scalar) {
  return vector_2d<B>(scalar * vec.x, scalar * vec.y);
}

#endif
