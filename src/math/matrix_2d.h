#ifndef MATRIX_2D_GUARD
#define MATRIX_2D_GUARD
#include <string>
#include <sstream>
#include "math/vector_2d.h"

/*
* Represents a 2x2 matrix for 2D operations.
* Induvidual elements, MIJ represents the element in the Ith row and Jth column
*/
template <class NUM_TYPE>
class matrix_2d {
public:
  NUM_TYPE M11, M12, M21, M22;

  /*
  * Defualt constructor
  */
  matrix_2d() : M11(static_cast<NUM_TYPE>(0)),
   M12(static_cast<NUM_TYPE>(0)),
    M21(static_cast<NUM_TYPE>(0)),
     M22(static_cast<NUM_TYPE>(0))
     { }

  matrix_2d(NUM_TYPE _M11,NUM_TYPE _M12,NUM_TYPE _M21,NUM_TYPE _M22)
  : M11(_M11), M12(_M12), M21(_M21), M22(_M22) { }

  /*This defines a copy constructor if A == NUM_TYPE
  * Otherwise it defines an explicit conversion using standard arithmetic conversions*/
  template<class A> explicit matrix_2d(const matrix_2d<A>& mat)
  : M11(static_cast<NUM_TYPE>(mat.M11)),
   M12(static_cast<NUM_TYPE>(mat.M12)),
    M21(static_cast<NUM_TYPE>(mat.M21)),
     M22(static_cast<NUM_TYPE>(mat.M22))
     { }

  matrix_2d<NUM_TYPE> transpose() const;
  matrix_2d<NUM_TYPE> inverse() const;
  NUM_TYPE determinant() const;
  operator std::string() const;
};

template<class A, class B> matrix_2d<A> operator+(matrix_2d<A> mat1, matrix_2d<B> mat2);
template<class A, class B> matrix_2d<A> operator-(matrix_2d<A> mat1, matrix_2d<B> mat2);
template<class A, class B> matrix_2d<A> operator*(matrix_2d<A> mat1, matrix_2d<B> mat2);
template<class A, class B> vector_2d<B> operator*(matrix_2d<A> mat, vector_2d<B> vec);
template<class A, class B> matrix_2d<B> operator*(matrix_2d<B> mat, A scalar);
template<class A, class B> matrix_2d<B> operator*(A scalar, matrix_2d<B> mat);

template<class A, class B> matrix_2d<B> operator*(matrix_2d<B> mat, A scalar) {
  return matrix_2d<B>(scalar * mat.M11, scalar * mat.M12, scalar * mat.M21, scalar * mat.M22);
}

template<class A, class B> matrix_2d<B> operator*(A scalar, matrix_2d<B> mat) {
  return matrix_2d<B>(scalar * mat.M11, scalar * mat.M12, scalar * mat.M21, scalar * mat.M22);
}

template<class NUM_TYPE> matrix_2d<NUM_TYPE> matrix_2d<NUM_TYPE>::transpose() const {
  return matrix_2d<NUM_TYPE>(M11, M21, M12, M22);
}

template<class NUM_TYPE> NUM_TYPE matrix_2d<NUM_TYPE>::determinant() const {
  return M11 * M22 - M12 * M21;
}

template<class NUM_TYPE> matrix_2d<NUM_TYPE> matrix_2d<NUM_TYPE>::inverse() const {
  NUM_TYPE det = determinant();
  return matrix_2d<NUM_TYPE>(M22/det, -M12/det, -M21/det, M11/det);
}

template<class NUM_TYPE> matrix_2d<NUM_TYPE>::operator std::string() const {
  std::ostringstream sstream;
  sstream << '\n' << '|' << M11 << ' ' << M12 << '|' << '\n';
  sstream << '|' << M21 << ' ' << M22 << '|' << '\n';
  return sstream.str();
}

template<class A, class B> matrix_2d<A> operator+(matrix_2d<A> mat1, matrix_2d<B> mat2) {
  return matrix_2d<A>(mat1.M11 + mat2.M11, mat1.M12 + mat2.M12, mat1.M21 + mat2.M21, mat1.M22 + mat2.M22);
}

template<class A, class B> matrix_2d<A> operator-(matrix_2d<A> mat1, matrix_2d<B> mat2) {
  return matrix_2d<A>(mat1.M11 - mat2.M11, mat1.M12 - mat2.M12, mat1.M21 - mat2.M21, mat1.M22 - mat2.M22);
}
template<class A, class B> matrix_2d<A> operator*(matrix_2d<A> mat1, matrix_2d<B> mat2) {
  return matrix_2d<A>(
    mat1.M11 * mat2.M11 + mat1.M12 * mat2.M21,
    mat1.M11 * mat2.M12 + mat1.M12 * mat2.M22,
    mat1.M21 * mat2.M11 + mat1.M22 * mat2.M21,
    mat1.M21 * mat2.M12 + mat1.M22 * mat2.M22);
}
template<class A, class B> vector_2d<B> operator*(matrix_2d<A> mat, vector_2d<B> vec) {
  return vector_2d<B>(
    mat.M11 * vec.x + mat.M12 * vec.y,
    mat.M21 * vec.x + mat.M22 * vec.y
  );
}

#endif
