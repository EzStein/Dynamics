#ifndef DYNSOLVER_GL_TEXT_RENDERER_H_
#define DYNSOLVER_GL_TEXT_RENDERER_H_

#include <glad/glad.h>

#include "gl/vertex_array.h"
#include "gl/buffer.h"
#include "gl/program.h"
#include "gl/sampler.h"

namespace dynsolver {
namespace gl {

class font;

// The text renderer class is used to draw text to the screen in a
// given font.
class text_renderer {
private:
  vertex_array vao;
  buffer vbo;
  sampler so;
  program prog;
  GLuint transformationUniform;
  GLuint samplerUniform;
  GLuint textColorUniform;
  GLuint vertexAttributeBinding;
  GLenum textureUnit;
  
public:
  text_renderer();

  // Draws the text to the screen at the given coordinates and in the given
  // font. The coordinates are the window space positions relative to the
  // bottom left corner of the screen.
  void render_text(const std::string& text, int x, int y, font&,
		   GLfloat scale = 1.0f,
		   GLfloat red = 0.0f, GLfloat green = 0.0f, GLfloat blue = 0.0f,
		   GLfloat alpha = 1.0f);
};
} // namespace gl
} // namespace dynsolver
#endif
