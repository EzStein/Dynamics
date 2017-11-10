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
public:

  gl_program(const char * vertexShaderPath, const char * fragmentShaderPath);
  GLint get_attribute_location(const char *) const;
  void bind() const;
  void setBoolUniform(const char* name, bool val) const;
  void setFloatUniform(const char* name, float val) const;
  void setIntUniform(const char* name, int val) const;


private:
  static bool read_file(const char * filePath, char* buffer, size_t size);
};
#endif
