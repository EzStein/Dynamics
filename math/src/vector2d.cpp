#include "math/vector2d.h"

#include <algorithm>
#include <cmath>

namespace math {
vector2d::vector2d() : vector(2) { }
vector2d::vector2d(double x, double y) : vector(2) {
  (*this)[0] = x;
  (*this)[1] = y;
}

double& vector2d::x() {
  return (*this)[0];
}

const double& vector2d::x() const {
  return (*this)[0];
}

double& vector2d::y() {
  return (*this)[1];
}

const double& vector2d::y() const {
  return (*this)[1];
}

double vector2d::line_segment_distance(const vector2d& p1, const vector2d& p2) const {
  double t = ((p2.x() - p1.x())*(x() - p1.x()) + (p2.y() - p1.y())*(y() - p1.y())) /
    ((p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y()));
  vector2d mid(t*(p2.x() - p1.x()) + p1.x(), t*(p2.y() - p1.y()) + p1.y());
  double minX = std::min(p1.x(), p2.x());
  double maxX = std::max(p1.x(), p2.x());
  double minY = std::min(p1.y(), p2.y());
  double maxY = std::max(p1.y(), p2.y());
  if(minX <= mid.x() && mid.x() <= maxX && minY <= mid.y() && mid.y() <= maxY) {
    return distance(mid);
  } else {
    if(mid.distance(p1) < mid.distance(p2)) {
      return distance(p1);
    } else {
      return distance(p2);
    }
  }
}

vector2d vector2d::operator+(const vector2d& other) const {
  vector2d vec(*this);
  vec += other;
  return vec;
}

vector2d vector2d::operator-(const vector2d& other) const {
  vector2d vec(*this);
  vec -= other;
  return vec;
}
} // namespace math

