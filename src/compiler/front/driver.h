#ifndef DRIVER_GUARD
#define DRIVER_GUARD
#include <sys/mman.h>
#include <cstddef>
#include <string>
#include <map>
#include <exception>
#include "compiler/front/buffer_attributes.h"



class driver {
public:
  /*NOTE: the function prototype is double func(double*)
  where the passed in array has
  t is the first element, followed by x, then y, then z etc..*/
  typedef double (*double_func_t)(const double*);

  driver();
  ~driver();

  /*
  * Move assignment and move constructor
  */
  driver(driver&&);
  driver& operator=(driver&&);

  /*Copy constructor and copy assignment are deleted*/
  driver(const driver&) = delete;
  driver& operator=(const driver&) = delete;



  void flush_all_buffers();
  void flush_unused_buffers();

  /*Compiles the string and returns a pointer to the compiled function.
  Any function returned by this method may be free'd using the mark_unused template.
  The buffer containing the function may not be deallocated immediately, but will be marked as unused,*/
  template<class FUNC_TYPE> FUNC_TYPE compile_as_function(std::string);

  /*Marks the provided function as unused. It may not be free'd immediately. The buffer
  space could be reused for a new function compilation.
  If the function was never allocated, this method does nothing*/
  template <class FUNC_TYPE> void mark_available(FUNC_TYPE);

private:
  typedef std::map<void*, buffer_attributes>::iterator iter_t;

  /*
  * Maps each allocated buffer to a struct containing its current attributes
  */
  std::map<void*, buffer_attributes> buffer_table;
};

#endif
