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


namespace gl {
namespace text_renderer_program {
extern const std::string kTransformationUniform;
extern const std::string kVertexShaderFilePath;
extern const std::string kFragmentShaderFilePath;
extern const std::string kTextColorUniform;
extern const std::string kSamplerUniform;
extern const GLuint kVertexPositionAttribute;
extern const GLuint kTexturePositionAttribute;
}
}
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
