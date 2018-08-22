#include "gl/vertex_array.h"

namespace gl {

vertex_array::vertex_array() {
  glGenVertexArrays(1, &handle);
}

vertex_array::~vertex_array() {
  // Does nothing if handle is zero.
  glDeleteVertexArrays(1, &handle);
}

vertex_array::vertex_array(vertex_array&& other) : handle(other.handle) {
  other.handle = 0;
}

vertex_array& vertex_array::operator=(vertex_array&& other) {
  if(this == &other) return *this;
  glDeleteVertexArrays(1, &handle);
  handle = other.handle;
  other.handle = 0;
  return *this;
}

GLuint vertex_array::get_handle() const {
  return handle;
}
} // namespace gl
