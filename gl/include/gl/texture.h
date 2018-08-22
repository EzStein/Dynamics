#ifndef GL_TEXTURE_H_
#define GL_TEXTURE_H_

#include <glad/glad.h>

namespace gl {

// This class handles creation and destruction of opengl textures.
class texture {
private:
  GLuint handle;
public:
  texture();
  ~texture();

  texture(const texture&) = delete;
  texture(texture&&);

  texture& operator=(const texture&) = delete;
  texture& operator=(texture&&);

  GLuint get_handle() const;
};
} // namespace gl

#endif
