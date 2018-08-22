#include "gl/program.h"

#include "gl/shader.h"

namespace gl {

program::program(const std::vector<shader>& shaders) {
  handle = glCreateProgram();
  for(const shader& shader : shaders) {
    glAttachShader(handle, shader.get_handle());
  }
  glLinkProgram(handle);
  for(const shader& shader : shaders) {
    glDetachShader(handle, shader.get_handle());
  }
  GLint success;
  glGetProgramiv(handle, GL_LINK_STATUS, &success);
  if(!success) {
    char infoLog[1024];
    glGetProgramInfoLog(handle, 1024, nullptr, infoLog);
    glDeleteProgram(handle);
    throw program_link_exception("Program failed to link: "
				 + std::string(infoLog));
  }
}

program::~program() {
  // Does nothing if program is zero.
  glDeleteProgram(handle);
}

program::program(program&& other) : handle(other.handle) {
  other.handle = 0;
}

program& program::operator=(program&& other) {
  if(this == &other) return *this;
  glDeleteProgram(handle);
  handle = other.handle;
  other.handle = 0;
  return *this;
}

GLuint program::get_handle() const {
  return handle; 
}
} // namespace gl

