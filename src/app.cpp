#include "app.h"

namespace dynsolver {
namespace app {
std::string generate_resource_path(std::vector<std::string> dirs, std::string file) {
  std::string path("");
  path += "/home/ezra/Documents/projects/hubbard_research/resources/";
  for(const std::string& dir : dirs) {
    path += dir;
    path += "/";
  }
  path += file;
  return path;
}
} // namespace app
} // namespace dynsolver
