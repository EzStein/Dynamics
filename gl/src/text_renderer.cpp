#include "gl/text_renderer.h"

#include <iostream>

#include "gl/font.h"
#include "gl/shader.h"
#include "util/util.h"

namespace gl {

const std::string text_renderer::kVertexShaderFilePath(VERTEX_SHADER_PATH);

const std::string text_renderer::kFragmentShaderFilePath(FRAGMENT_SHADER_PATH);

const std::string text_renderer::kTransformationUniform("transformation");
const std::string text_renderer::kTextColorUniform("textColor");
const std::string text_renderer::kSamplerUniform("sampler");
const GLuint text_renderer::kVertexPositionAttribute(0);
const GLuint text_renderer::kTexturePositionAttribute(1);

std::vector<shader> text_renderer::generate_shaders() {
  std::vector<shader> shaders;
  std::string code(util::read_file(kVertexShaderFilePath));
  shaders.push_back(shader(code, GL_VERTEX_SHADER));
  code = util::read_file(kFragmentShaderFilePath);
  shaders.push_back(shader(code, GL_FRAGMENT_SHADER));
  return shaders;
}

text_renderer::text_renderer() : prog(generate_shaders()),
				 vbo(nullptr, 16 * sizeof(GLfloat), GL_DYNAMIC_DRAW),
				 vertexAttributeBinding(0), textureUnit(0) {
  // Initialize Program Info
  transformationUniform =
    glGetUniformLocation(prog.get_handle(), kTransformationUniform.c_str());
  samplerUniform =
    glGetUniformLocation(prog.get_handle(), kSamplerUniform.c_str());
  textColorUniform =
    glGetUniformLocation(prog.get_handle(), kTextColorUniform.c_str());
  glUseProgram(prog.get_handle());
  glUniform1i(samplerUniform, textureUnit);
  glUseProgram(prog.get_handle());
  
  // Initialize VAO
  glBindVertexArray(vao.get_handle());
  glEnableVertexAttribArray(kVertexPositionAttribute);
  glVertexAttribFormat(kVertexPositionAttribute, 2, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(kVertexPositionAttribute, vertexAttributeBinding);
    
  glEnableVertexAttribArray(kTexturePositionAttribute);
  glVertexAttribFormat(kTexturePositionAttribute,
		       2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat));
  glVertexAttribBinding(kTexturePositionAttribute, vertexAttributeBinding);
  
  glBindVertexBuffer(vertexAttributeBinding, vbo.get_handle(), 0, 4 * sizeof(GLfloat));
  glBindVertexArray(0);
  
  // Initialize Sampler
  glSamplerParameteri(so.get_handle(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glSamplerParameteri(so.get_handle(), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glSamplerParameteri(so.get_handle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glSamplerParameteri(so.get_handle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindSampler(textureUnit, so.get_handle());
}

void text_renderer::render_text(const std::string& text, int x, int y,
				const font& font,
				GLfloat fontSize,
				GLfloat red, GLfloat green, GLfloat blue,
				GLfloat alpha) const {
  GLfloat buffer[16] = {
    0.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f
  };

  GLint viewportData[4];
  glGetIntegerv(GL_VIEWPORT, viewportData);
  GLfloat widthFloat = static_cast<GLfloat>(viewportData[2]);
  GLfloat heightFloat = static_cast<GLfloat>(viewportData[3]);
  // Row major matrix which converts pixels to NDC coordinates.
  GLfloat transformation[16] = {
    2.0f/widthFloat, 0.0f, 0.0f, -1.0f,
    0.0f, 2.0f/heightFloat, 0.0f, -1.0f,
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

  GLfloat scale = fontSize / font.get_base_height();
  
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
