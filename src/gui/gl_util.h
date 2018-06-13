#ifndef DYNSOLVER_GUI_GL_UTIL_H_
#define DYNSOLVER_GUI_GL_UTIL_H_

#include <glad/glad.h>
#include <string>

// gl_util.h is a collection of utility functions used in for working with
// opengl. Opengl maintains a global (thread-local) state and thus it does
// not lend itself to object oriented programming. Thus each function is
// standalone, and contained within the gl namespace.

namespace dynsolver {
namespace gui {
namespace gl {

// Compiles and links a shader program whose vertex and fragment shaders are
// the code contained in the passed in strings. Returns the id of the
// created program. Requires that the opengl function pointers have been
// loaded.
GLuint compile_program(const std::string& vertexShader,
		       const std::string& fragmentShader);

// Performs the same function as the above function but also compiles and links
// a geometry shader.
GLuint compile_program(const std::string& vertexShader,
		       const std::string& fragmentShader,
		       const std::string& geometryShader);

} // namespace gl
} // namespace gui
} // namespace dynsolver
#endif
