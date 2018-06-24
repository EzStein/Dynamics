#ifndef DYNSOLVER_CONSTANTS_H_
#define DYNSOLVER_CONSTANTS_H_

#include <glad/glad.h>
#include <string>

// This header file declares global constant variables.
namespace dynsolver {
namespace constants {

extern const std::string kDefaultFontFilePath;

namespace app {
extern const int kGLMajorVersion;
extern const int kGLMinorVersion;
} // namespace app

} // namespace constants
} // namespace dynsolver

#endif
