#include "constants.h"

#include <vector>

#include "app.h"

namespace dynsolver {
namespace constants {

const std::string kDefaultFontFilePath
(::dynsolver::app::generate_resource_path
 (std::vector<std::string>{"fonts"}, "OpenSans-Regular.ttf"));

namespace app {
const int kGLMajorVersion(4);
const int kGLMinorVersion(3);
} // namespace app

} // namespace constants
} // namespace dynsolver
