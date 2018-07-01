#include "math/quaternion.h"

#include <cmath>

#include "math/matrix_3x3.h"
#include "math/vector3d.h"

namespace dynsolver {
namespace math {

quaternion::quaternion(const vector3d& axis, double angle) : vector(4, 0.0) {
  (*this)[0] = axis.x() * std::sin(angle / 2);
  (*this)[1] = axis.y() * std::sin(angle / 2);
  (*this)[2] = axis.z() * std::sin(angle / 2);
  (*this)[3] = std::cos(angle / 2);
}

quaternion::quaternion(double x, double y, double z, double w) : vector(4, 0.0) {
  (*this)[0] = x;
  (*this)[1] = y;
  (*this)[2] = z;
  (*this)[3] = w;
}

double& quaternion::x() {
  return (*this)[0];
}

const double& quaternion::x() const {
  return (*this)[0];
}

double& quaternion::y() {
  return (*this)[1];
}

const double& quaternion::y() const {
  return (*this)[1];
}

double& quaternion::z() {
  return (*this)[2];
}

const double& quaternion::z() const {
  return (*this)[2];
}
  
double& quaternion::w() {
  return (*this)[3];
}

const double& quaternion::w() const {
  return (*this)[3];
}

matrix_3x3 quaternion::as_rotation_matrix() const {
  matrix_3x3 rotation;
  rotation[0][0] = 1 - 2*y()*y() - 2*z()*z();
  rotation[0][1] = 2*x()*y()-2*w()*z();
  rotation[0][2] = 2*x()*z()+2*w()*y();

  rotation[1][0] = 2*x()*y()+2*w()*z();
  rotation[1][1] = 1 - 2*x()*x() - 2*z()*z();
  rotation[1][2] = 2*y()*z()-2*w()*x();

  rotation[2][0] = 2*x()*z()-2*w()*y();
  rotation[2][1] = 2*y()*z()+2*w()*x();
  rotation[2][2] = 1 - 2*x()*x() - 2*y()*y();
  
  return rotation;
}

void quaternion::operator*=(const quaternion& q) {
  double newX = w() * q.x() + x() * q.w() + y() * q.z() - z() * q.y();
  double newY = w() * q.y() + y() * q.w() + z() * q.x() - x() * q.z();
  double newZ = w() * q.z() + z() * q.w() + x() * q.y() - y() * q.x();
  double newW = w() * q.w() - x() * q.x() - y() * q.y() - z() * q.z();
  x() = newX;
  y() = newY;
  z() = newZ;
  w() = newW;
}

quaternion operator*(quaternion lhs, const quaternion& rhs) {
  lhs *= rhs;
  return lhs;
}
} // namespace math
} // namespace dynsolver
