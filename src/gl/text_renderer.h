
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
  // Marked mutable since it is never directly exposed.
  mutable buffer vbo;
  sampler so;
  program prog;
  GLuint transformationUniform;
  GLuint samplerUniform;
  GLuint textColorUniform;
  GLuint vertexAttributeBinding;
  GLenum textureUnit;

  static const std::string kVertexShaderFilePath;
  static const std::string kFragmentShaderFilePath;
  static const std::string kTransformationUniform;
  static const std::string kTextColorUniform;
  static const std::string kSamplerUniform;
  static const GLuint kVertexPositionAttribute;
  static const GLuint kTexturePositionAttribute;
  
public:
  text_renderer();

  // Draws the text to the screen at the given coordinates and in the given
  // font. The coordinates are the window space positions relative to the
  // bottom left corner of the screen. fontSize is the size in pixels
  // of the font. Generally this just means the height of the rendered text.
  void render_text(const std::string& text, int x, int y, const font&,
		   GLfloat fontSize = 20,
		   GLfloat red = 0.0f, GLfloat green = 0.0f, GLfloat blue = 0.0f,
		   GLfloat alpha = 1.0f) const;
  
private:
  std::vector<shader> generate_shaders();
};
} // namespace gl
} // namespace dynsolver
#endif
