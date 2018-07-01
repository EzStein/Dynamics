#include "math/vector.h"

#include <cassert>
#include <cmath>

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

namespace vector_ops {
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
} // namespace vector_ops

double angle(const vector& v1, const vector& v2) {
  assert(v1.size() == v2.size());
  return std::acos(dot(v1, v2) / (v1.norm() * v2.norm()));
}

double dot(const vector& v1, const vector& v2) {
  assert(v1.size() == v2.size());
  double acc = 0;
  for(int i = 0; i != v1.size(); ++i) {
    acc += v1[i] * v2[i];
  }
  return acc;
}
} // namespace math
} // namespace dynslover
