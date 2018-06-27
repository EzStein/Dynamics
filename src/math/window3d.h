#ifndef DYNSOLVER_MATH_WINDOW3D_H_
#define DYNSOLVER_MATH_WINDOW3D_H_

#include "math/vector3d.h"

namespace dynsolver {
namespace math {

class square_matrix;

// Represents a window into three dimensional viewport. Provides methods for
// retrieving transformation matrices for use in opengl. In particular, this
// represents a camera in world space coordinates with a position, view direction,
// and up direction.
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

  // This is the aspect ratio of the GUI window that the 3d view is rendered into.
  // It is given by width divided by height. Must be strictly positive.
  double aspectRatio;

public:
  // Initializes this window. The up parameter can be any vector which is
  // not colinear with viewDirection. It does not have to be orthogonal
  // to viewDirection.
  window3d(const vector3d& position, const vector3d& viewDirection,
	   const vector3d& up,
	   double nearZ, double farZ, double aspectRatio);

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
  
  // Computes and returns the camera to clip matrix. This converts points in camera,
  // space to points in clip space.
  square_matrix camera_to_clip() const;

  // Computes and returns the world to camer matrix. This matrix is used to convert
  // world coordinates by translating the camera to the origin and rotating it until
  // it faces down the negative z axis and is pointing up in the positive y axis.
  square_matrix world_to_camera() const;
};

// Produces a 4x4 translation matrix which translates points according to the
// provided 3d vector.
square_matrix generate_translation_matrix(const vector3d&);
} // namespace math
} // namespace dynslover

#endif
