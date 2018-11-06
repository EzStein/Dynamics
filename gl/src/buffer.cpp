#include "gl/buffer.h"

#include <sstream>
#include <stdexcept>
#include <iostream>
namespace gl {
namespace {
void process_gl_errors() {
  GLenum err;
  bool erro = false;
  while((err = glGetError()) != GL_NO_ERROR) {
    erro = true;
    std::stringstream sstream;
    sstream << std::hex << err;
    std::cout << "OpenGL exception: 0x" << sstream.str() << std::endl;
  }
  if(erro) {
    throw std::runtime_error("OpenGL exception: GL_ERROR");
  }
}
}
buffer::buffer(const unsigned char* data, size_t _size, GLenum storage)
    : storage(storage), size(_size), handle(0) {
  //throw std::runtime_error("test");
  static int count = 0;
  ++count;
  glGenBuffers(1, &handle);
  process_gl_errors();
  glBindBuffer(GL_COPY_WRITE_BUFFER, handle);

  process_gl_errors();
  glBufferData(GL_COPY_WRITE_BUFFER, size, data, storage);
  process_gl_errors();
  glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

buffer::~buffer() {
  // Does nothing if handle is zero.
  glDeleteBuffers(1, &handle);
}

buffer::buffer(const buffer& other) :
    storage(other.storage), size(other.size) {
  glGenBuffers(1, &handle);
  glBindBuffer(GL_COPY_WRITE_BUFFER, handle);
  glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, storage);
  glBindBuffer(GL_COPY_READ_BUFFER, other.handle);
  glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
  glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
  glBindBuffer(GL_COPY_READ_BUFFER, 0);
}

buffer::buffer(buffer&& other) : handle(other.handle),
                                 storage(other.storage), size(other.size) {
  // Prevent the vbo from being destroyed immediatly when other::~buffer
  // is called
  other.handle = 0;
}

buffer& buffer::operator=(const buffer& other) {
  if(&other == this) return *this;
  glBindBuffer(GL_COPY_WRITE_BUFFER, handle);
  if(size < other.size || other.storage != storage) {
    glBufferData(GL_COPY_WRITE_BUFFER, other.size, nullptr, other.storage);
    size = other.size;
  }
  storage = other.storage;
  glBindBuffer(GL_COPY_READ_BUFFER, other.handle);
  glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, other.size);
  glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
  glBindBuffer(GL_COPY_READ_BUFFER, 0);
  return *this;
}

buffer& buffer::operator=(buffer&& other) {
  if(&other == this) return *this;
  if(handle != 0) {
    glDeleteBuffers(1, &handle);
  }

  storage = other.storage;
  size = other.size;
  handle = other.handle;

  // Prevent the vbo from being destroyed when it enters
  // other::~buffer()
  other.handle = 0;
  return *this;
}

void buffer::set_data(const unsigned char* data, size_t newSize) {
  glBindBuffer(GL_COPY_WRITE_BUFFER, handle);
  if(newSize > size) {
    // The buffer must increase so we reallocate.
    glBufferData(GL_COPY_WRITE_BUFFER, newSize, data, storage);
    size = newSize;
  } else {
    // We can reuse the same buffer
    glBufferSubData(GL_COPY_WRITE_BUFFER, 0, newSize, data);
  }
  glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

GLuint buffer::get_handle() const {
  return handle;
}

size_t buffer::get_capacity() const {
  return size;
}
} // namespace gl
