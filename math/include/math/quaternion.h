#ifndef MATH_QUATERNION_H_
#define MATH_QUATERNION_H_

#include "math/vector.h"

namespace math {
class matrix_3x3;
class vector3d;

// Represents a quaternion viewed as a vector in 4 dimensions.
class quaternion : public vector {
public:
  // Constructs a quaternion from an axis and an angle.
  // The quaternion is a unit quaternion if axis is a unit vector.
  quaternion(const vector3d& axis, double angle);

  // Constructs a quaternion from its four component values.
  quaternion(double x, double y, double z, double w);

  double& x();
  const double& x() const;

  double& y();
  const double& y() const;

  double& z();
  const double& z() const;
  
  double& w();
  const double& w() const;

  // Converts this quaternion to a 3x3 rotation matrix.
  // This is only meaningful if the quaternion was constructed using
  // an axis and an angle.
  matrix_3x3 as_rotation_matrix() const;

  // Performs a quaternionic multiplication.
  void operator*=(const quaternion&);
};

// Performs a quaternionic multiplication on the provided two quaternions.
quaternion operator*(quaternion lhs, const quaternion& rhs);

} // namespace math

#endif
