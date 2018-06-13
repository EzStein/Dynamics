#ifndef DYNSOLVER_CONSTANTS_H_
#define DYNSOLVER_CONSTANTS_H_

#include <glad/glad.h>
#include <string>

// This header file declares global constant variables.
namespace dynsolver {
namespace constants {

namespace app {
extern const std::string kProjectPath;
extern const int kGLMajorVersion;
extern const int kGLMinorVersion;
} // namespace app

namespace vertex_shader {
extern const std::string kCode;
extern const GLuint kPositionAttribute;
extern const std::string kTransformationUniform;
extern const GLuint kVertexBinding;
} // namespace vertex_shader

namespace fragment_shader {
extern const std::string kCode;
extern const std::string kColorUniform;
} // namespace fragment_shader

} // namespace constants
} // namespace dynsolver

#endif
