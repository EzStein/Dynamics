#include "gl/texture.h"

namespace gl {

texture::texture() {
  glGenTextures(1, &handle);
}

texture::~texture() {
  // Does nothing if handle is zero.
  glDeleteTextures(1, &handle);
}

texture::texture(texture&& other) : handle(other.handle) {
  other.handle = 0;
}

texture& texture::operator=(texture&& other) {
  if(this == &other) return *this;
  glDeleteTextures(1, &handle);
  handle = other.handle;
  other.handle = 0;
  return *this;
}

GLuint texture::get_handle() const {
  return handle;
}
} // namespace gl
