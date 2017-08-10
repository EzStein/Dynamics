#ifndef DRIVER_GUARD
#define DRIVER_GUARD
#include <sys/mman.h>
#include <cstddef>
#include <string>

class driver {
public:
  typedef double (*float_function)(double);

  /*Compiles the string and returns a pointer to the compiled function.
  Any function returned by this method may be free'd using the delete_function template*/
  static float_function compile_as_float_function(std::string);

  template <class FUNC>
  static void delete_function(FUNC, size_t);
};

#endif
