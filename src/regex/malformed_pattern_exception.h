#ifndef DYNSOLVER_REGEX_MALFORMED_PATTERN_EXCEPTION_H_
#define DYNSOLVER_REGEX_MALFORMED_PATTERN_EXCEPTION_H_

#include <stdexcept>
#include <string>

namespace dynsolver {
namespace regex {

// This class is used to indicate errors in the creation of regular expression
// objects. The class regex throws this exception when its constructor
// is called with an invalid pattern string.
class malformed_pattern_exception : public std::runtime_error {
 public:
  // Constructs this exception with the provided message which is
  // returned by a call to what().
  explicit malformed_pattern_exception(const std::string& message)
      : runtime_error(message) { }
};

} // namespace regex
} // namespace dynsolver
#endif
