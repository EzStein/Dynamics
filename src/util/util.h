#ifndef UTIL_UTIL_GUARD
#define UTIL_UTIL_GUARD
#include <string>
/*Cotains utility functions to be used by the program in the namespace util
* In general these functions are utilities that are not part of any class but are useful
* for many classes*/
namespace util {
  /*
  * Removes any whitespace in the string
  * by modifying the original string.
  */
  void strip_white_space(std::string&);

  /*
  * Converts the string which may contain a decimal to a double.
  * Assumes there is no whitespace involved.
  * This function performs no checks to make sure the string is well formed
  */
  double string_to_double(const std::string&);

}
#endif
