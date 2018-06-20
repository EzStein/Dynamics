#ifndef DYNSOLVER_GL_VERTEX_ARRAY_H_
#define DYNSOLVER_GL_VERTEX_ARRAY_H_

#include <glad/glad.h>

namespace dynsolver {
namespace gl {

// This class handles creation and destruction of opengl vertex_arrays.
class vertex_array {
private:
  GLuint handle;
public:
  vertex_array();
  ~vertex_array();

  vertex_array(const vertex_array&) = delete;
  vertex_array(vertex_array&&);

  vertex_array& operator=(const vertex_array&) = delete;
  vertex_array& operator=(vertex_array&&);

  GLuint get_handle() const;
};
} // namespace gl
} // namespace dynsolver

#endif
