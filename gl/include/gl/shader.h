#ifndef GL_SHADER_H_
#define GL_SHADER_H_

#include <glad/glad.h>
#include <stdexcept>

namespace gl {

class shader {
private:
  GLuint handle;
public:
  shader(const std::string& code, GLenum type);
  ~shader();

  shader(const shader&) = delete;
  shader(shader&&);

  shader& operator=(const shader&) = delete;
  shader& operator=(shader&&);

  GLuint get_handle() const;
};

class shader_compile_exception : public std::runtime_error {
public:
  // Constructs this exception with a message. The message is returned by a
  // call to what().
  explicit shader_compile_exception(const std::string& message)
    : std::runtime_error(message) { }
};
} // namespace gl
#endif
