#ifndef DYNSOLVER_GL_SAMPLER_H_
#define DYNSOLVER_GL_SAMPLER_H_

#include <glad/glad.h>

namespace dynsolver {
namespace gl {

// This class handles creation and destruction of opengl samplers.
class sampler {
private:
  GLuint handle;
public:
  sampler();
  ~sampler();

  sampler(const sampler&) = delete;
  sampler(sampler&&);

  sampler& operator=(const sampler&) = delete;
  sampler& operator=(sampler&&);

  GLuint get_handle() const;
};
} // namespace gl
} // namespace dynsolver

#endif
