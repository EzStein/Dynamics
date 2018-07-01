#include "math/vector3d.h"

namespace dynsolver {
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

vector3d cross(const vector3d& v1, const vector3d& v2) {
  return vector3d(v1.y()*v2.z() - v1.z()*v2.y(),
		  v1.z()*v2.x() - v1.x()*v2.z(),
		  v1.x()*v2.y() - v1.y()*v2.x());
}

vector3d operator-(const vector3d& lhs, const vector3d& rhs) {
  vector3d sub(lhs.x() - rhs.x(),
	       lhs.y() - rhs.y(),
	       lhs.z() - rhs.z());
  return sub;
}
} // namespace math
} // namespace dynsolver
