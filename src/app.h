#ifndef DYNSOLVER_APP_H_
#define DYNSOLVER_APP_H_

#include <string>
#include <vector>

namespace dynsolver {
namespace app {
// Generates the file path in a platform specific way to the resource requested.
// File is the name of the file,
// dirs represents in order the list of directories which will be appended with
// the file separator characters.
std::string generate_resource_path(std::vector<std::string> dirs, std::string file);
} // namespace app
} // namespace dynsolver

#endif
