#ifndef MATH_WINDOW3D_H_
#define MATH_WINDOW3D_H_

#include "math/vector3d.h"

namespace math {

class matrix_4x4;
class matrix_3x3;
class vector2d;

// Represents a window into three dimensional viewport. Provides methods for
// retrieving transformation matrices for use in opengl. In particular, this
// represents a camera in world space coordinates with a position,
// view direction, and up direction.
class window3d {
private:
  vector3d position;

  // The view direction is the direction that the camera is pointing in.
  // Must be normalized.
  vector3d viewDirection;

  // The up direction is the direction that indicates up for the camera.
  // It must be orthogonal to viewDirection.
  // Must be normalized.
  vector3d upDirection;

  // The nearZ and farZ variables indicate the z
  // clipping distance from the camera. nearZ must be less than farZ, both
  // must be strictly positive.
  double nearZ;
  double farZ;

  // This is the aspect ratio of the GUI window that the 3d view is rendered
  // into. It is given by width divided by height. Must be strictly positive.

  // The width and height given in pixels.
  double width;
  double height;

public:
  // Constructs a window whose camera is at the origin. Up is the positive y
  // axis, and it is pointing down the neagtive z axis. The near and far planes
  // are 1 and 10 respectively and the aspect ration is 1:1.
  window3d();
  
  // Initializes this window. The up parameter can be any vector which is
  // not colinear with viewDirection. It does not have to be orthogonal
  // to viewDirection.
  window3d(const vector3d& position, const vector3d& viewDirection,
	   const vector3d& up,
	   double nearZ, double farZ, double width, double height);

  // Getters/Setters
  const vector3d& get_position() const;
  void set_position(const vector3d&);
  const vector3d& get_view_direction() const;
  const vector3d& get_up_direction() const;
  double get_near_z() const;
  void set_near_z(double);
  double get_far_z() const;
  void set_far_z(double);
  double get_aspect_ratio() const;
  void set_aspect_ratio(double);

  // Returns the width and height in pixels.
  double get_width() const;
  double get_height() const;

  void set_width(double newWidth);
  void set_height(double newHeight);

  // Rotates the view according to a mouse drag. The two input vectors are
  // the start and end of the mouse drag in window coordinates.
  void rotate(const vector2d&, const vector2d&);
  
  // Computes and returns the camera to clip matrix. This converts points
  // in camera, space to points in clip space.
  matrix_4x4 camera_to_clip() const;

  // Computes and returns the world to camera matrix. This matrix is used to
  // convert world coordinates by translating the camera to the origin and
  // rotating it until it faces down the negative z axis and is pointing up
  // in the positive y axis.
  matrix_4x4 world_to_camera() const;

  // Returns the 3x3 matrix representing the rotation component of the
  // transformation from world to camera coordinates.
  matrix_3x3 world_to_camera_rotation() const;

  // Returns the 4x4 matrix representing the translation component of the
  // transformation from world to camera coordinates.
  matrix_4x4 world_to_camera_translation() const;

  // Converts window coordinates to the the
  // normalized device coordinates. The normalized device coordinates (x, y)
  // correspond to the camera coordinates (x, y, -1) which lie on the projective
  // plane.
  vector2d window_to_ndc(const vector2d&) const;
};

// Produces a 4x4 translation matrix which translates points according to the
// provided 3d vector.
matrix_4x4 generate_translation_matrix(const vector3d&);
} // namespace math

#endif
