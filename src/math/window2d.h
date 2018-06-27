#ifndef DYNSOLVER_MATH_WINDOW2D_H_
#define DYNSOLVER_MATH_WINDOW2D_H_
#include "math/vector2d.h"

namespace dynsolver {
namespace math {

class square_matrix;

// Represents a window into a 2d plane that has both a size in pixels
// a spanning size of the plane and a location.
// Note that pixels are given realative to the bottom left corner
// of the plane. In particular, upwards is the POSITIVE y direction.
class window2d {
 private:
  // The size in pixels of window. The x coordinate of size is the width,
  // and the y coordinate of size is the height.
  vector2d size;

  // The real spanning distance of this window. The distance spanned by
  // the window in the x and y directions.
  vector2d span;

  // This is the position of the bottom right corner of the window in the plane.
  vector2d position;

 public:
  // Constructs a window with the given information.
  window2d(vector2d size, vector2d span, vector2d position);
  
  // Returns the pixel associated with the real coordinate provided.
  vector2d pixel_coordinate_of(vector2d real) const;
  
  // Returns the real cordinated associated with the given pixel.
  vector2d real_coordinate_of(vector2d pixel) const;

  // True if the pixel corrdinates are nonnegative and are
  // within the bounds of size exclusive.
  bool contains_pixel(vector2d pixel) const;

  // Returns true if the real coordinate provided is within the span
  // of the window.
  bool contains_real(vector2d real) const;

  // Translates the window by the number of pixels specified in pixel.
  void move_pixel(vector2d pixel);

  // Translates the window by the real value provided.
  void move_real(vector2d real);

  void set_span(const vector2d& newSpan);

  // Scales the window by scale about the real point provided.
  // The resulting window has a the same size as before, but the
  // new x span is scale.x() * span.x()
  // and new y span of scale.y() * span.y().
  // The point real remains fixed in the new window. That is, it is
  // associated with the same pixel as before.
  void scale_real(vector2d scale, vector2d real);

  // Scales the window by scale about the pixel point provided.
  void scale_pixel(vector2d scale, vector2d pixel);

  const vector2d& get_size() const;
  void set_size(const vector2d&);
  
  const vector2d& get_position() const;
  const vector2d& get_span() const;

  // Returns the center of the this window in real coordinates.
  vector2d get_center() const;

  // This transformation matrix is a 4x4 matrix which maps
  // real coordinates to normalized device coordinates for use in opengl.
  square_matrix real_to_ndc();

  // This transformation matrix is a 4x4 matrix which maps
  // pixel coordinate to normalized device coordinates for use in opengl.
  // In pixel coordinates, the origin is the lower left corner of the screen.
  square_matrix pixel_to_ndc();
};
} // namespace dynsolver
} // namespace math
#endif