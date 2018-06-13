#include "gui/gl_util.h"

#include <cassert>
#include <string>

namespace dynsolver {
namespace gui {
namespace gl {

GLuint compile_program(const std::string& vertexShaderCode,
		       const std::string& fragmentShaderCode) {
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char* buf = vertexShaderCode.c_str();
  glShaderSource(vertexShader, 1, &buf, NULL);
  glCompileShader(vertexShader);

  GLint success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  assert(success);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  buf = fragmentShaderCode.c_str();
  glShaderSource(fragmentShader, 1, &buf, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  assert(success);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  assert(success);

  glDetachShader(program, vertexShader);
  glDeleteShader(vertexShader);

  glDetachShader(program, fragmentShader);
  glDeleteShader(fragmentShader);
  
  return program;
}

GLuint compile_program(const std::string& vertexShaderCode,
		       const std::string& fragmentShaderCode,
		       const std::string& geometryShaderCode) {
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char* buf = vertexShaderCode.c_str();
  glShaderSource(vertexShader, 1, &buf, NULL);
  glCompileShader(vertexShader);

  GLint success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  assert(success);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  buf = fragmentShaderCode.c_str();
  glShaderSource(fragmentShader, 1, &buf, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  assert(success);

  GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
  buf = geometryShaderCode.c_str();
  glShaderSource(geometryShader, 1, &buf, NULL);
  glCompileShader(geometryShader);

  glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
  assert(success);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glAttachShader(program, geometryShader);
  glLinkProgram(program);
  
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  assert(success);

  glDetachShader(program, vertexShader);
  glDeleteShader(vertexShader);
  glDetachShader(program, geometryShader);
  glDeleteShader(geometryShader);
  glDetachShader(program, fragmentShader);
  glDeleteShader(fragmentShader);
  
  return program;
}

} // namespace gl
} // namespace gui
} // namespace dynsolver
