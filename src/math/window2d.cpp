#include "math/window2d.h"

#include "math/matrix_4x4.h"

namespace dynsolver {
namespace math {

window2d::window2d() : window2d(vector2d(100, 100), vector2d(2, 2),
				vector2d(-1, -1)) { }

window2d::window2d(const vector2d& size, const vector2d& span,
		   const vector2d& position) :
  size(size), span(span), position(position) { }

vector2d window2d::pixel_of(const vector2d& real) const {
  vector2d pixel;
  pixel.x() = (real.x() - position.x()) * size.x() / span.x();
  pixel.y() = (real.y() - position.y()) * size.y() / span.y();
  return pixel;
}

vector2d window2d::real_coordinate_of(const vector2d& pixel) const {
  vector2d real;
  real.x() = pixel.x() * span.x() / size.x() + position.x();
  real.y() = pixel.y() * span.y() / size.y() + position.y();
  return real;
}

const vector2d& window2d::get_size() const {
  return size;
}

const vector2d& window2d::get_span() const {
  return span;
}

void window2d::set_size(const vector2d& newSize) {
  size = newSize;
}

void window2d::set_span(const vector2d& newSpan) {
  span = newSpan;
}

const vector2d& window2d::get_position() const{
  return position;
}

bool window2d::contains_pixel(const vector2d& pixel) const {
  return (pixel.x() >= 0 && pixel.x() < size.x()
          && pixel.y() >= 0 && pixel.y() < size.y());
}

bool window2d::contains_real(const vector2d& real) const {
  return contains_pixel(pixel_of(real));
}

void window2d::move_pixel(const vector2d& pixel) {
  position.x() += pixel.x() * span.x() / size.x();
  position.y() += pixel.y() * span.y() / size.y();
}

void window2d::move_real(const vector2d& real) {
  position.x() += real.x();
  position.y() += real.y();
}

void window2d::scale_pixel(const vector2d& scale, const vector2d& pixel) {
  scale_real(scale, real_coordinate_of(pixel));
}

void window2d::scale_real(const vector2d& scale, const vector2d& real) {
  span.x() *= scale.x();
  span.y() *= scale.y();
  position.x() = (position.x() - real.x()) * scale.x() + real.x();
  position.y() = (position.y() - real.y()) * scale.y() + real.y();
}

vector2d window2d::get_center() const {
  vector2d center;
  center.x() = position.x() + span.x()/2;
  center.y() = position.y() + span.y()/2;
  return center;
}

void window2d::set_position(const vector2d& val) {
  position = val;
}

matrix_4x4 window2d::real_to_ndc() const {
  vector2d center(get_center());
  matrix_4x4 mat;
  mat[0][0] = 2.0/span.x();
  mat[0][3] = -center.x()*mat[0][0];

  mat[1][1] = 2.0/span.y();
  mat[1][3] = -center.y()*mat[1][1];
  return mat;
}

matrix_4x4 window2d::pixel_to_ndc() const {
  vector2d center(get_center());
  matrix_4x4 mat;
  mat[0][0] = 2.0/size.x();
  mat[0][3] = -1;

  mat[1][1] = 2.0/size.y();
  mat[1][3] = -1;
  return mat;
}
} // namespace math
} // namespace dynsolver
