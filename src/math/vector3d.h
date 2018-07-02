#ifndef DYNSOLVER_MATH_VECTOR3D_H_
#define DYNSOLVER_MATH_VECTOR3D_H_

#include "math/vector.h"

namespace dynsolver {
namespace math {

// Specializes the vector for 3 dimensions.
class vector3d : public vector {
public:
  // Constructs the vector with the three values.
  vector3d(double x, double y, double z);

  double& x();
  const double& x() const;

  double& y();
  const double& y() const;

  double& z();
  const double& z() const;

  // Normalizes this vector
  void normalize();

  // Crosses this vector with the other.
  vector3d& cross(const vector3d& other);

  vector3d& operator+=(const vector3d& other);
  vector3d& operator-=(const vector3d& other);
  vector3d& operator*=(double scal);

  // Returns the cross product of the two vectors.
  static vector3d cross(vector3d, const vector3d&);
};

vector3d operator+(vector3d lhs, const vector3d& rhs);
vector3d operator-(vector3d lhs, const vector3d& rhs);
vector3d operator*(double scal, vector3d rhs);
vector3d operator*(vector3d lhs, double scal);
} // namespace math		
} // namespace dynslover
#endif
