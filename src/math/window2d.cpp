#include "math/window2d.h"

#include "math/square_matrix.h"

namespace dynsolver {
namespace math {

window2d::window2d(vector2d size, vector2d span, vector2d position) :
    size(size), span(span), position(position) { }

vector2d window2d::pixel_coordinate_of(vector2d real) const {
  vector2d pixel;
  pixel.x() = (real.x() - position.x()) * size.x() / span.x();
  pixel.y() = (real.y() - position.y()) * size.y() / span.y();
  return pixel;
}

vector2d window2d::real_coordinate_of(vector2d pixel) const {
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

bool window2d::contains_pixel(vector2d pixel) const {
  return (pixel.x() >= 0 && pixel.x() < size.x()
          && pixel.y() >= 0 && pixel.y() < size.y());
}

bool window2d::contains_real(vector2d real) const {
  return contains_pixel(pixel_coordinate_of(real));
}

void window2d::move_pixel(vector2d pixel) {
  position.x() += pixel.x() * span.x() / size.x();
  position.y() += pixel.y() * span.y() / size.y();
}

void window2d::move_real(vector2d real) {
  position.x() += real.x();
  position.y() += real.y();
}

void window2d::scale_pixel(vector2d scale, vector2d pixel) {
  scale_real(scale, real_coordinate_of(pixel));
}

void window2d::scale_real(vector2d scale, vector2d real) {
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

square_matrix window2d::real_to_ndc() {
  vector2d center(get_center());
  square_matrix mat(4, 0.0);
  mat[0][0] = 2.0/span.x();
  mat[0][3] = -center.x()*mat[0][0];

  mat[1][1] = 2.0/span.y();
  mat[1][3] = -center.y()*mat[1][1];

  mat[2][2] = 1;
  mat[3][3] = 1;
  return mat;
}

square_matrix window2d::pixel_to_ndc() {
  vector2d center(get_center());
  square_matrix mat(4, 0.0);
  mat[0][0] = 2.0/size.x();
  mat[0][3] = -1;

  mat[1][1] = 2.0/size.y();
  mat[1][3] = -1;

  mat[2][2] = 1;
  mat[3][3] = 1;
  return mat;
}
} // namespace math
} // namespace dynsolver
