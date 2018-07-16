#ifndef DYNSOLVER_MATH_WINDOW2D_H_
#define DYNSOLVER_MATH_WINDOW2D_H_
#include "math/vector2d.h"

namespace dynsolver {
namespace math {

class square_matrix;
class matrix_4x4;

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

  // This is the position of the bottom left corner of the window in the plane.
  vector2d position;

 public:
  // Constructs a window viewing into the -1 to 1 range in both the x and
  // y axes. The pixel range is 100 by 100.
  window2d();
  
  // Constructs a window with the given information.
  window2d(const vector2d& size, const vector2d& span,
	   const vector2d& position);
  
  // Returns the pixel associated with the real coordinate provided.
  vector2d pixel_of(const vector2d& real) const;
  
  // Returns the real cordinated associated with the given pixel.
  vector2d real_coordinate_of(const vector2d& pixel) const;

  // True if the pixel corrdinates are nonnegative and are
  // within the bounds of size exclusive.
  bool contains_pixel(const vector2d& pixel) const;

  // Returns true if the real coordinate provided is within the span
  // of the window.
  bool contains_real(const vector2d& real) const;

  // Translates the window by the number of pixels specified in pixel.
  void move_pixel(const vector2d& pixel);

  // Translates the window by the real value provided.
  void move_real(const vector2d& real);

  void set_span(const vector2d& newSpan);

  // Scales the window by scale about the real point provided.
  // The resulting window has a the same size as before, but the
  // new x span is scale.x() * span.x()
  // and new y span of scale.y() * span.y().
  // The point real remains fixed in the new window. That is, it is
  // associated with the same pixel as before.
  void scale_real(const vector2d& scale, const vector2d& real);

  // Scales the window by scale about the pixel point provided.
  void scale_pixel(const vector2d& scale, const vector2d& pixel);

  const vector2d& get_size() const;
  void set_size(const vector2d&);

  void set_position(const vector2d&);
  
  const vector2d& get_position() const;
  const vector2d& get_span() const;

  // Returns the center of the this window in real coordinates.
  vector2d get_center() const;

  // This transformation matrix is a 4x4 matrix which maps
  // real coordinates to normalized device coordinates for use in opengl.
  matrix_4x4 real_to_ndc() const;

  // This transformation matrix is a 4x4 matrix which maps
  // pixel coordinate to normalized device coordinates for use in opengl.
  // In pixel coordinates, the origin is the lower left corner of the screen.
  matrix_4x4 pixel_to_ndc() const;
};
} // namespace dynsolver
} // namespace math
#endif
