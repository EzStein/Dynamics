#include "gl/font.h"

#include <glad/glad.h>
#include <cassert>
#include <iostream>

#include "free_type_library_singleton.h"

namespace dynsolver {
namespace gl {

font::font(const std::string& file) :
  library(free_type_library_singleton::get_free_type_library()),
  baseHeight(48) {
  int failure = FT_New_Face(library, file.c_str(), 0, &face);
  assert(!failure);
  FT_Set_Pixel_Sizes(face, 0, baseHeight);
}

font::~font() {
  FT_Done_Face(face);
}

void font::get_character_data(unsigned long characterCode,
			size_t& width, size_t& height,
			size_t& bearingX, size_t& bearingY,
			size_t& advance, GLuint& texture) {
  std::unordered_map<unsigned long, character>::const_iterator iter =
    characters.find(characterCode);
  if(iter != characters.end()) {
    texture = iter->second.textureObj.get_handle();
    advance = iter->second.advance;
    bearingY = iter->second.bearingY;
    bearingX = iter->second.bearingX;
    width = iter->second.width;
    height = iter->second.height;
    return;
  }
  FT_Load_Char(face, characterCode, FT_LOAD_RENDER);
  character charData;
  charData.width = face->glyph->bitmap.width;
  charData.height = face->glyph->bitmap.rows;
  charData.bearingX = face->glyph->bitmap_left;
  charData.bearingY = face->glyph->bitmap_top;
  charData.advance = face->glyph->advance.x;

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
  GLuint textureHandle = charData.textureObj.get_handle();
  glBindTexture(GL_TEXTURE_2D, textureHandle);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, charData.width, charData.height,
	       0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
  glBindTexture(GL_TEXTURE_2D, 0);

  width = charData.width;
  height = charData.height;
  bearingX = charData.bearingX;
  bearingY = charData.bearingY;
  advance = charData.advance;
  texture = textureHandle;
  characters.insert(std::make_pair(characterCode, std::move(charData)));
}

FT_UInt font::get_base_height() const {
  return baseHeight;
}
} // namespace gl
} // namespace dynsolver
