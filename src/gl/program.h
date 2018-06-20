#ifndef DYNSOLVER_GL_PROGRAM_H_
#define DYNSOLVER_GL_PROGRAM_H_

#include <glad/glad.h>
#include <vector>
#include <stdexcept>

namespace dynsolver {
namespace gl {

class shader;

// Represents an opengl program object that handles construction,
// and destruction only.
class program {
private:
  GLuint handle;
public:
  program(const std::vector<shader>& shaders);
  ~program();

  // Copy and move constructors
  program(const program&) = delete;
  program(program&&);

  // Copy and move assignment
  program& operator=(const program&) = delete;
  program& operator=(program&&);

  GLuint get_handle() const;
};

class program_link_exception : public std::runtime_error {
public:
  // Constructs this exception with a message. The message is returned by a
  // call to what().
  explicit program_link_exception(const std::string& message)
    : std::runtime_error(message) { }
};
} // namespace gl
} // namespace dynsolver
#endif
