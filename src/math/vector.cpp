#include "math/vector.h"

#include <cassert>

namespace dynsolver {
namespace math {

vector::vector(int size) : matrix(size, 1) { }
vector::vector(int size, double val) : matrix(size, 1, val) { }

double& vector::operator[](int index) {
  return matrix::operator[](index)[0];
}

const double& vector::operator[](int index) const {
  return matrix::operator[](index)[0];
}

int vector::size() const {
  return rows;
}

vector operator+(vector lhs, const vector& rhs) {
  lhs += rhs;
  return lhs;
}
vector operator-(vector lhs, const vector& rhs) {
  lhs -= rhs;
  return lhs;
}
vector operator*(matrix lhs, const vector& rhs) {
  lhs *= rhs;
  vector ret(rhs.size());
  for(int i = 0; i != rhs.size(); ++i) {
    ret[i] = lhs[i][0];
  }
  return ret;
}
vector operator*(double scalar, vector mat) {
  mat *= scalar;
  return mat;
}
vector operator*(vector mat, double scalar) {
  mat *= scalar;
  return mat;
}
vector operator/(vector mat, double scalar) {
  mat /= scalar;
  return mat;
}
} // namespace math
} // namespace dynslover
