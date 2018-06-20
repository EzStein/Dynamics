#include "gl/buffer.h"

namespace dynsolver {
namespace gl {
buffer::buffer(const unsigned char* data, size_t size, GLenum storage)
  : storage(storage), size(size), handle(0) {
  glGenBuffers(1, &handle);
  glBindBuffer(GL_COPY_WRITE_BUFFER, handle);
  glBufferData(GL_COPY_WRITE_BUFFER, size, data, storage);
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
  glDeleteBuffers(1, &handle);
  
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
} // namespace dynsolver
