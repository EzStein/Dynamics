#ifndef GL_BUFFER_H_
#define GL_BUFFER_H_

#include <glad/glad.h>
#include <cstddef>

namespace gl {

// This class manages a any buffer object whether it be an array buffer,
// element buffer, or uniform buffer, etc...
// The class uses RAII style to implement construction, and destruction
// of these buffer objects. It otherwise provides no other buffer semantics
// except for copying data to the object.
//
// It is important to only use only the methods in this class for sending data
// to the buffer. The handle to the buffer object provided by get_handle()
// should only be used to read data from the object.
//
// NOTE: this object freely uses the GL_COPY_WRITE_BUFFER and GL_COPY_READ_BUFFER.
// All function calls INCLUDING copies and moves may change which buffer is bound
// to these targets.
class buffer {
private:
  GLenum storage;
  GLuint handle;
  size_t size;

  // Class invariant: Handle always points to a buffer whose memory has been
  // allocated.
public:
  // Constructs a buffer, allocates storage for it on the GPU,
  // and copies size bytes of data from the pointer data to it.
  buffer(const unsigned char* data, size_t size, GLenum storage);

  // Destroys the object
  ~buffer();

  // Copy and move constructors
  buffer(const buffer&);
  buffer(buffer&&);

  // Copy and move assignment
  buffer& operator=(const buffer&);
  buffer& operator=(buffer&&);

  // Returns the handle to the buffer object.
  // NOTE: this index should be used only to read the
  // buffer. Do not use it to modify its contents.
  GLuint get_handle() const;

  // Returns the number of bytes stored in the buffer.
  size_t get_capacity() const;

  // Copies the data pointed to by data into the VBO.
  // The number of bytes copied is given by size.
  // Requires that size is nonzero. Depending on whether the buffer is large
  // enough, this call will either reallocate the buffer or simply copy
  // data into it.
  void set_data(const unsigned char* data, size_t size);
};
} // namepace gl
#endif
