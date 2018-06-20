#include "gl/text_renderer.h"

#include "gl/font.h"
#include "gl/shader.h"
#include "constants.h"
#include "util/util.h"

namespace dynsolver {
namespace gl {

namespace {
std::vector<shader> generate_shaders() {
  std::vector<shader> shaders;
  std::string code(util::read_file(constants::gl::text_renderer_program::kVertexShaderFilePath));
  shaders.push_back(shader(code, GL_VERTEX_SHADER));
  code = util::read_file(constants::gl::text_renderer_program::kFragmentShaderFilePath);
  shaders.push_back(shader(code, GL_FRAGMENT_SHADER));
  return shaders;
}
}

text_renderer::text_renderer() : prog(generate_shaders()),
				 vbo(nullptr, 16 * sizeof(GLfloat), GL_DYNAMIC_DRAW),
				 vertexAttributeBinding(0), textureUnit(0) {
  // Initialize Program Info
  transformationUniform =
    glGetUniformLocation(prog.get_handle(),
			 constants::gl::text_renderer_program::kTransformationUniform.c_str());
  samplerUniform =
    glGetUniformLocation(prog.get_handle(),
			 constants::gl::text_renderer_program::kSamplerUniform.c_str());
  textColorUniform =
    glGetUniformLocation(prog.get_handle(),
			 constants::gl::text_renderer_program::kTextColorUniform.c_str());
  glUseProgram(prog.get_handle());
  glUniform1i(samplerUniform, textureUnit);
  glUseProgram(prog.get_handle());
  
  // Initialize VAO
  glBindVertexArray(vao.get_handle());
  glEnableVertexAttribArray(constants::gl::text_renderer_program::kVertexPositionAttribute);
  glVertexAttribFormat(constants::gl::text_renderer_program::kVertexPositionAttribute,
			 2, GL_FLOAT, GL_FALSE, 0);
  glEnableVertexAttribArray(constants::gl::text_renderer_program::kTexturePositionAttribute);
  glVertexAttribBinding(constants::gl::text_renderer_program::kVertexPositionAttribute,
			   vertexAttributeBinding);
  glVertexAttribFormat(constants::gl::text_renderer_program::kTexturePositionAttribute,
		       2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat));

  glVertexAttribBinding(constants::gl::text_renderer_program::kTexturePositionAttribute,
			   vertexAttributeBinding);
  glBindVertexBuffer(vertexAttributeBinding, vbo.get_handle(), 0, 4 * sizeof(GLfloat));
  glBindVertexArray(0);
  
  // Initialize Sampler
  glSamplerParameteri(so.get_handle(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glSamplerParameteri(so.get_handle(), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glSamplerParameteri(so.get_handle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glSamplerParameteri(so.get_handle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindSampler(textureUnit, so.get_handle());
}

void text_renderer::render_text(const std::string& text, int x, int y, font& font,
				GLfloat scale,
				GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
  GLfloat buffer[16] = {
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f
  };

  GLint widthPixel, heightPixel;
  glGetIntegeri_v(GL_VIEWPORT, 2, &widthPixel);
  glGetIntegeri_v(GL_VIEWPORT, 3, &heightPixel);
  GLfloat widthFloat = static_cast<GLfloat>(widthPixel);
  GLfloat heightFloat = static_cast<GLfloat>(heightPixel);
  // Row major matrix which converts pixels to NDC coordinates.
  GLfloat transformation[16] = {
    2/widthFloat, 0.0f, 0.0f, -1.0f,
    0.0f, 2/heightFloat, 0.0f, -1.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindVertexArray(vao.get_handle());
  glUseProgram(prog.get_handle());
  glUniform4f(textColorUniform, red, green, blue, alpha);
  glUniformMatrix4fv(transformationUniform, 1, GL_TRUE, transformation);
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  
  for(char c : text) {
    size_t width, height, bearingX, bearingY, advance;
    GLuint texture;
    font.get_character_data(c, width, height, bearingX, bearingY, advance, texture);

    GLfloat bottomLeftX = x + bearingX * scale;
    GLfloat bottomLeftY = y - height * scale + bearingY * scale;
    
    // Bottom Left
    buffer[0] = bottomLeftX;
    buffer[1] = bottomLeftY;

    // Top Left
    buffer[4] = bottomLeftX;
    buffer[5] = bottomLeftY + height * scale;

    // Bottom Right
    buffer[8] = bottomLeftX + width * scale;
    buffer[9] = bottomLeftY;

    // Top Right
    buffer[12] = bottomLeftX + width * scale; 
    buffer[13] = bottomLeftY + height * scale;

    glBindTexture(GL_TEXTURE_2D, texture);
    vbo.set_data(reinterpret_cast<unsigned char*>(buffer), 16 * sizeof(GLfloat));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    x += (advance * scale) / 64;
  }
  glBindVertexArray(0);
}
}
}
