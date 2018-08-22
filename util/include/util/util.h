#ifndef UTIL_H_
#define UTIL_H_
#include <string>
// Cotains utility functions to be used by the program in the namespace util
// In general these functions are utilities that are not part of any class but are useful
// for many classes
namespace util {

// Removes any whitespace in the string
// by modifying the original string.
void strip_white_space(std::string&);

// Converts the string which may contain a decimal to a double.
// Assumes there is no whitespace involved.
// This function performs no checks to make sure the string is well formed
//
double string_to_double(const std::string&);
  
bool is_integer(const std::string&);
long string_to_long(const std::string&);

// Taken form stack overflow:
// https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
bool has_only_digits(const std::string&);

// Reads the entirety of the file into memory and returns it
// as a string. Throws an exception on failure.
std::string read_file(const std::string& path);

// Creates a string of the double with the given number of decimal points.
std::string double_to_string(double x, int precision);

} // namespace util
#endif
