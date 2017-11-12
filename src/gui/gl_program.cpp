#include "gl_program.h"

gl_program::gl_program(const std::string& _vertexShaderPath, const std::string& _fragmentShaderPath)
: vertexShaderPath(_vertexShaderPath), fragmentShaderPath(_fragmentShaderPath) { }

void gl_program::compile() {
  /*Load and compile shaders*/
  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  /*Allocate a buffer to read the file into*/
  size_t bufferSize = 1024;
  char * buffer = static_cast<char*>(malloc(bufferSize*sizeof(char)));

  /*READ the file into the buffer*/
  if(!read_file(vertexShaderPath.c_str(), buffer, bufferSize)) {
    std::cout << "BUFFER TOO SMALL TO READ IN FILE" << std::endl;
    return;
  }

  glShaderSource(vertexShader, 1, &buffer, NULL);
  glCompileShader(vertexShader);

  constexpr size_t logSize = 512;
  int success;
  char infoLog[logSize];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if(!success) {
    glGetShaderInfoLog(vertexShader, logSize, NULL, infoLog);
    std::cout << "GL shader compilation error VERTEX: " << infoLog << std::endl;
  }

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  if(!read_file(fragmentShaderPath.c_str(), buffer, bufferSize)) {
    std::cout << "BUFFER TOO SMALL TO READ IN FILE" << std::endl;
    return;
  }

  glShaderSource(fragmentShader, 1, &buffer, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(fragmentShader, logSize, NULL, infoLog);
    std::cout << "GL shader compilation error FRAGMENT: " << infoLog << std::endl;
  }

  programID = glCreateProgram();
  glAttachShader(programID, vertexShader);
  glAttachShader(programID, fragmentShader);
  glLinkProgram(programID);
  glGetProgramiv(programID, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(programID, logSize, NULL, infoLog);
    std::cout << "GL shader linking error: " << infoLog << std::endl;
  }
  free(buffer);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void gl_program::bind() const {
  glUseProgram(programID);
}

void gl_program::set_bool_uniform(const char * name, bool val) const {
  glUniform1i(glGetUniformLocation(programID, name), static_cast<int>(val));
}

void gl_program::set_int_uniform(const char * name, int val) const {
  glUniform1i(glGetUniformLocation(programID, name), val);
}

void gl_program::set_float_uniform(const char * name, float val) const {
  glUniform1f(glGetUniformLocation(programID, name), val);
}

void gl_program::set_float_mat4_uniform(const char * name,  bool transpose, float * val) const {
  glUniformMatrix4fv(glGetUniformLocation(programID, name),1, transpose, val);
}

/*
* Reads a file into the character buffer provided,
* appending a null terminating character if necessary.
* No more than size characters will be read into the buffer.
* Returns true if the whole file was read. False if it was truncated.
*/
bool gl_program::read_file(const char * filePath, char* buffer, size_t size) {
  FILE* file = fopen(filePath, "rb");
  if(!file)
    throw "No such file or directory: " + std::string(filePath);

  /*Seeks to the end of the file to get file size*/
  fseek(file, 0, SEEK_END);
  const size_t fileSize = ftell(file);
  rewind(file);

  /*We read in either the file size, or size-1 whichever is smaller*/
  size_t bytesToRead = size - 1 < fileSize?size-1:fileSize;
  fread(buffer, sizeof(char), bytesToRead, file);
  fclose(file);
  /*We append a null terminating character*/
  buffer[bytesToRead] = '\x00';
  return bytesToRead == fileSize;
}

GLint gl_program::get_attribute_location(const char * name) const {
  return glGetAttribLocation(programID, name);
}
