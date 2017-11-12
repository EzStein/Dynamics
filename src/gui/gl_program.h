#ifndef GL_PROGRAM_GUARD
#define GL_PROGRAM_GUARD
#include "glad/glad.h"
#include <string>
#include <iostream>
/*
* This class can load an openGl program
* using the path to the vertex and fragment shader.
* Note, the function pointers to the gl functions must be loaded
* before using this class.
*/
class gl_program {
private:
  GLuint programID;
  std::string vertexShaderPath, fragmentShaderPath;
public:

  gl_program(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
  GLint get_attribute_location(const char *) const;
  void bind() const;
  void compile();
  void set_bool_uniform(const char* name, bool val) const;
  void set_float_uniform(const char* name, float val) const;
  void set_int_uniform(const char* name, int val) const;
  void set_float_mat4_uniform(const char * name,  bool transpose, float * val) const;


private:
  static bool read_file(const char * filePath, char* buffer, size_t size);
};
#endif
