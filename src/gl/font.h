#ifndef DYNSOLVER_GL_FONT_H_
#define DYNSOLVER_GL_FONT_H_

#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "gl/texture.h"

namespace dynsolver {
namespace gl {

// This class loads a font using FreeType and dynamically,
// generates textures for each character of the font whenever
// one calls get_character_data()
class font {
private:
  class character {
  public:
    texture textureObj;
    size_t advance;
    size_t width;
    size_t height;
    size_t bearingX;
    size_t bearingY;
  };
  // A map from unicode code points to the corresponding character data,
  // given by the font loaded.
  std::unordered_map<unsigned long, character> characters;

  FT_Face face;
  FT_Library& library;

  FT_UInt baseHeight;

public:
  font(const std::string& file);
  ~font();

  font(const font&) = delete;
  font(font&&) = delete;

  font& operator=(const font&) = delete;
  font& operator=(font&&) = delete;

  // Retrieves the character data for the provided unicode character.
  // The results are passed to the caller by reference parameters.
  void get_character_data(unsigned long character,
			  size_t& width, size_t& height,
			  size_t& bearingX, size_t& bearingY,
			  size_t& advance, GLuint& texture);
  
  FT_UInt get_base_height() const;
};


}
}
#endif
