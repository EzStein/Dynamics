#include "math/window3d.h"

#include <cassert>
#include <iostream>

#include "math/vector3d.h"
#include "math/quaternion.h"
#include "math/vector2d.h"
#include "math/matrix_3x3.h"
#include "math/matrix_4x4.h"

namespace math {

window3d::window3d() : window3d(vector3d(0,0,0), vector3d(0,0,-1),
				vector3d(0,1,0),
				1, 10, 100, 100) {}

window3d::window3d(const vector3d& position, const vector3d& viewDirection,
		   const vector3d& up,
		   double nearZ, double farZ, double width, double height) :
  position(position), viewDirection(viewDirection),
  upDirection(vector3d::cross(vector3d::cross(viewDirection, up), viewDirection)),
  nearZ(nearZ), farZ(farZ), width(width), height(height) {
  this->viewDirection.normalize();
  this->upDirection.normalize();
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
  return width / height;
}

double window3d::get_width() const {
  return width;
}

double window3d::get_height() const {
  return height;
}

void window3d::set_width(double newWidth) {
  width = newWidth;
}

void window3d::set_height(double newHeight) {
  height = newHeight;
}

void window3d::rotate(const vector2d& start, const vector2d& end) {
  const vector3d negZ(0,0,-1);
  vector2d startNdc(window_to_ndc(start));
  vector2d endNdc(window_to_ndc(end));
  vector3d startCamera(startNdc.x(), startNdc.y(), -1);
  vector3d endCamera(endNdc.x(), endNdc.y(), -1);
  double angle(-vector::angle(endCamera, startCamera));
  vector3d diff = startCamera - endCamera;
  diff.normalize();
  
  matrix_3x3 cameraToWorldRot(world_to_camera_rotation());
  cameraToWorldRot.invert();
  quaternion rotation(cameraToWorldRot * vector3d::cross(diff, negZ), angle);
  matrix_3x3 transform(rotation.as_rotation_matrix());
  viewDirection = transform * viewDirection;
  upDirection = transform * upDirection;
}

vector2d window3d::window_to_ndc(const vector2d& window) const {
  vector2d ndc(0,0);
  ndc.x() = 2 * window.x() / width - 1;
  ndc.y() = 2 * window.y() / height - 1;
  return ndc;
}

matrix_3x3 window3d::world_to_camera_rotation() const {
  const vector3d negZ(0,0,-1);
  const vector3d posY(0,1,0);
  vector3d rotation1Axis(vector3d::vector3d::cross(viewDirection, negZ));
  double angle(vector::angle(viewDirection, negZ));
  quaternion rotation1(rotation1Axis, angle);
  vector3d newUp(rotation1.as_rotation_matrix() * upDirection);
  vector3d rotation2Axis(vector3d::vector3d::cross(newUp, posY));
  angle = vector::angle(newUp, posY);
  quaternion rotation2(rotation2Axis, angle);
  return (rotation2 * rotation1).as_rotation_matrix();
}

matrix_4x4 window3d::camera_to_clip() const {
  const double scaleX = 1.0;
  const double scaleY = 1.0;
  matrix_4x4 matrix;
  matrix[0][0] = (scaleX / width) * height;
  matrix[1][1] = scaleY;
  matrix[2][2] = (farZ + nearZ) / (nearZ - farZ);
  matrix[2][3] = (2 * farZ * nearZ) / (nearZ - farZ);
  matrix[3][2] = -1;
  matrix[3][3] = 0;
  return matrix;
}

matrix_4x4 window3d::world_to_camera_translation() const {
  vector3d translation(-position.x(), -position.y(), -position.z());
  return generate_translation_matrix(translation);
}

matrix_4x4 window3d::world_to_camera() const {
  return world_to_camera_rotation().augment_to_4x4()
    * world_to_camera_translation();
}

matrix_4x4 generate_translation_matrix(const vector3d& vec) {
  matrix_4x4 matrix;
  matrix[0][3] = vec.x();
  matrix[1][3] = vec.y();
  matrix[2][3] = vec.z();
  return matrix;
}
} // namespace math
