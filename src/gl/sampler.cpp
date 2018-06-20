#include "gl/sampler.h"

namespace dynsolver {
namespace gl {

sampler::sampler() {
  glGenSamplers(1, &handle);
}

sampler::~sampler() {
  // Does nothing if handle is zero.
  glDeleteSamplers(1, &handle);
}

sampler::sampler(sampler&& other) : handle(other.handle) {
  other.handle = 0;
}

sampler& sampler::operator=(sampler&& other) {
  if(this == &other) return *this;
  glDeleteSamplers(1, &handle);
  handle = other.handle;
  other.handle = 0;
  return *this;
}

GLuint sampler::get_handle() const {
  return handle;
}
} // namespace gl
} // namespace dynsolver
