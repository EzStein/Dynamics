#include "gl/shader.h"

namespace gl {
shader::shader(const std::string& code, GLenum type) {
  handle = glCreateShader(type);
  const char* buffer(code.c_str());
  glShaderSource(handle, 1, &buffer, nullptr);
  glCompileShader(handle);
  GLint success;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
  if(!success) {
    char infoLog[1024];
    glGetShaderInfoLog(handle, 1024, nullptr, infoLog);
    glDeleteShader(handle);
    throw shader_compile_exception("Shader failed to compile: "
				   + std::string(infoLog));
  }
}

shader::~shader() {
  glDeleteShader(handle);
}

shader::shader(shader&& other) : handle(other.handle) {
  other.handle = 0;
}

shader& shader::operator=(shader&& other) {
  if(this == &other) return *this;
  glDeleteShader(handle);
  handle = other.handle;
  other.handle = 0;
  return *this;
}

GLuint shader::get_handle() const {
  return handle;
}
} // namespace gl

int main(void) {
  gl::shader shader("", GL_STATIC_DRAW);
  return -1;
}
