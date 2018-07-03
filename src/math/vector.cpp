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

vector vector::projection(const vector& other) const {
  return other * (dot(*this, other) / dot(other, other));
}

vector vector::operator+(const vector& rhs) const {
  vector vec(*this);
  vec += rhs;
  return vec;
}

vector vector::operator-(const vector& rhs) const {
  vector vec(*this);
  vec -= rhs;
  return vec;
}

vector vector::operator*(double scal) const {
  vector vec(*this);
  vec *= scal;
  return vec;
}

vector vector::operator/(double scal) const {
  vector vec(*this);
  vec /= scal;
  return vec;
}

} // namespace math
} // namespace dynslover
