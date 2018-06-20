#ifndef DYNSOLVER_FREE_TYPE_LIBRARY_SINGLETON_H_
#define DYNSOLVER_FREE_TYPE_LIBRARY_SINGLETON_H_

#include <ft2build.h>
#include FT_FREETYPE_H

namespace dynsolver {

// A singleton that provides us with an initialized free type library.
// Consider making this a thread-local singleton!
class free_type_library_singleton {
private:
  static free_type_library_singleton singleton;
  
  free_type_library_singleton();

  FT_Library ftLibrary;
  
public:
  static FT_Library& get_free_type_library();

  ~free_type_library_singleton();
  
  free_type_library_singleton(const free_type_library_singleton&) = delete;
  free_type_library_singleton(free_type_library_singleton&&) = delete;

  free_type_library_singleton& operator=(const free_type_library_singleton&) = delete;
  free_type_library_singleton& operator=(free_type_library_singleton&&) = delete;
};
}

#endif
