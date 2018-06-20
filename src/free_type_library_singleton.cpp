#include "free_type_library_singleton.h"

#include <cassert>

namespace dynsolver {

free_type_library_singleton free_type_library_singleton::singleton;

FT_Library& free_type_library_singleton::get_free_type_library() {
  return singleton.ftLibrary;
}

free_type_library_singleton::free_type_library_singleton() {
  int failure = FT_Init_FreeType(&ftLibrary);
  assert(!failure);
}

free_type_library_singleton::~free_type_library_singleton() {
  FT_Done_FreeType(ftLibrary);
}

} // namespace dynsolver
