#ifndef DYNSOLVER_MATH_VECTOR2D_H_
#define DYNSOLVER_MATH_VECTOR2D_H_

#include "math/vector.h"

namespace dynsolver {
namespace math {
// Contains helper classes
// Represents a point in 2 dimensions.
class vector2d : public vector {
 public:
  // Constructs a vector2d without initializing its element.
  vector2d();
  
  // Constructs a point at position (x, y).
  // May also be used to represent a scaling vector, etc.
  vector2d(double x, double y);

  // Allows for mutable access of x and y.
  const double& x() const;
  double& x();
    
  const double& y() const;
  double& y();

  // Computes the distance from this point to the line segment
  // connected the two provided points. Note that this computes the
  // distance to a line segment, not a line.
  double line_segment_distance(const vector2d&, const vector2d&) const;
};
} // namespace math
} // namespace dynsolver
#endif
