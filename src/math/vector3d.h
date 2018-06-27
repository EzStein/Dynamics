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
};

// Returns the cross product of the two vectors.
vector3d cross(const vector3d&, const vector3d&);

} // namespace math		
} // namespace dynslover
#endif
