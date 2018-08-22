#ifndef GL_SAMPLER_H_
#define GL_SAMPLER_H_

#include <glad/glad.h>

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

#endif
