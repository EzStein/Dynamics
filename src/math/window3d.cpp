#include "math/window3d.h"

#include <cassert>

#include "math/square_matrix.h"
#include "math/quaternion.h"
#include "math/vector.h"

namespace dynsolver {
namespace math {

window3d::window3d(const vector3d& position, const vector3d& viewDirection,
		   const vector3d& up,
		   double nearZ, double farZ, double aspectRatio) :
  position(position), viewDirection(viewDirection),
  upDirection(cross(cross(up, viewDirection), viewDirection)),
  nearZ(nearZ), farZ(farZ), aspectRatio(aspectRatio) {
  viewDirection.norm();
  upDirection.norm();
}

const vector3d& window3d::get_position() const {
  return position;
}
void window3d::set_position(const vector3d& val) {
  position = val;
}
const vector3d& window3d::get_view_direction() const {
  return viewDirection;
}
const vector3d& window3d::get_up_direction() const {
  return upDirection;
}
double window3d::get_near_z() const {
  return nearZ;
}
void window3d::set_near_z(double val) {
  assert(val > 0 && val < farZ);
  nearZ = val;
}
double window3d::get_far_z() const {
  return farZ;
}
void window3d::set_far_z(double val) {
  assert(val > nearZ);
  farZ = val;
}
double window3d::get_aspect_ratio() const {
  return aspectRatio;
}
void window3d::set_aspect_ratio(double val) {
  assert(val > 0);
  aspectRatio = val;
}

square_matrix window3d::camera_to_clip() const {
  const double scale = 1.0;
  square_matrix matrix(4, 0.0);
  matrix[0][0] = scale / aspectRatio;
  matrix[1][1] = scale;
  matrix[2][2] = (farZ + nearZ) / (nearZ - farZ);
  matrix[2][3] = (2 * farZ * nearZ) / (nearZ - farZ);
  matrix[3][2] = -1;
  return matrix;
}

square_matrix window3d::world_to_camera() const {
  const vector3d negZ(0,0,-1);
  const vector3d posY(0,1,0);
  vector3d vector(cross(viewDirection, negZ));
  double angle(math::angle(viewDirection, negZ));
  quaternion rotation1(vector, angle);
  math::vector upVec(4, 0.0);
  upVec[0] = upDirection.x();
  upVec[1] = upDirection.y();
  upVec[2] = upDirection.z();
  upVec[3] = 1;
  math::vector newUpVec(rotation1.as_rotation_matrix() * upVec);
  vector3d newUp(newUpVec[0], newUpVec[1], newUpVec[2]);
  vector = cross(newUp, posY);
  angle = math::angle(newUp, posY);
  quaternion rotation2(vector, angle);
  vector3d translation(-position.x(), -position.y(), -position.z());
  square_matrix translationMatrix(generate_translation_matrix(translation));
  return (rotation2 * rotation1).as_rotation_matrix() * translationMatrix;
}

square_matrix generate_translation_matrix(const vector3d& vec) {
  square_matrix matrix(4, 0.0);
  matrix[0][0] = 1;
  matrix[1][1] = 1;
  matrix[2][2] = 1;
  matrix[3][3] = 1;
  matrix[0][3] = vec.x();
  matrix[1][3] = vec.y();
  matrix[2][3] = vec.z();
  return matrix;
}
} // namespace math
} // namespace dynslover
