#include "math/vector3d.h"

namespace math {

vector3d::vector3d(double x, double y, double z) : vector(3) {
  (*this)[0] = x;
  (*this)[1] = y;
  (*this)[2] = z;
}

double& vector3d::x() {
  return (*this)[0];
}

const double& vector3d::x() const {
  return (*this)[0];
}

double& vector3d::y() {
  return (*this)[1];
}

const double& vector3d::y() const {
  return (*this)[1];
}

double& vector3d::z() {
  return (*this)[2];
}

const double& vector3d::z() const {
  return (*this)[2];
}

void vector3d::normalize() {
  double norm = vector::norm();
  x() /= norm;
  y() /= norm;
  z() /= norm;
}

vector3d& vector3d::operator+=(const vector3d& other) {
  vector::operator+=(other);
  return *this;
}
vector3d& vector3d::operator-=(const vector3d& other) {
  vector::operator-=(other);
  return *this;
}

vector3d& vector3d::operator*=(double scal) {
  vector::operator*=(scal);
  return *this;
}

vector3d& vector3d::cross(const vector3d& v) {
  double xVal = y()*v.z() - z()*v.y();
  double yVal = z()*v.x() - x()*v.z();
  double zVal = x()*v.y() - y()*v.x();
  x() = xVal;
  y() = yVal;
  z() = zVal;
  return *this;
}

vector3d vector3d::cross(vector3d v1, const vector3d& v2) {
  return v1.cross(v2);
}

vector3d operator+(vector3d lhs, const vector3d& rhs) {
  return lhs += rhs;
}
vector3d operator-(vector3d lhs, const vector3d& rhs) {
  return lhs -= rhs;
}
vector3d operator*(double scalar, vector3d vec) {
  return vec *= scalar;
}
vector3d operator*(vector3d vec, double scalar) {
  return vec *= scalar;
}
} // namespace math
