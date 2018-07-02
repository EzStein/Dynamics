#include "math/vector.h"

#include <cassert>
#include <cmath>

namespace dynsolver {
namespace math {
vector::vector() : matrix() { }
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

double vector::distance(const vector& other) const {
  assert(size() == other.size());
  double acc = 0.0;
  for(int i = 0; i != size(); ++i) {
    double change = (*this)[i] - other[i];
    acc += change * change;
  }
  return std::sqrt(acc);
}


vector& vector::operator+=(const vector& other) {
  matrix::operator+=(other);
  return *this;
}
vector& vector::operator-=(const vector& other) {
  matrix::operator-=(other);
  return *this;
}
vector& vector::operator*=(double scal) {
  matrix::operator*=(scal);
  return *this;
}
vector& vector::operator/=(double scal) {
  matrix::operator/=(scal);
  return *this;
}

double vector::angle(const vector& v1, const vector& v2) {
  assert(v1.size() == v2.size());
  return std::acos(dot(v1, v2) / (v1.norm() * v2.norm()));
}

double vector::dot(const vector& v1, const vector& v2) {
  assert(v1.size() == v2.size());
  double acc = 0;
  for(int i = 0; i != v1.size(); ++i) {
    acc += v1[i] * v2[i];
  }
  return acc;
}

vector operator+(vector lhs, const vector& rhs) {
  return lhs += rhs;
}
vector operator-(vector lhs, const vector& rhs) {
  return lhs -= rhs;
}
vector operator*(double scalar, vector vec) {
  return vec *= scalar;
}
vector operator*(vector vec, double scalar) {
  return vec *= scalar;
}
vector operator/(vector vec, double scalar) {
  return vec /= scalar;
}
namespace vector_ops {
vector operator*(matrix lhs, const vector& rhs) {
  lhs *= rhs;
  vector ret(rhs.size());
  for(int i = 0; i != rhs.size(); ++i) {
    ret[i] = lhs[i][0];
  }
  return ret;
}
}
} // namespace math
} // namespace dynslover
