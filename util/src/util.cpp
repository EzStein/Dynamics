#include <cstdlib>
#include <climits>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cassert>
#ifdef IS_WINDOWS
#include <windows.h>
#include <Shlwapi.h>
// remember to link against shlwapi.lib
// in VC++ this can be done with
#pragma comment(lib, "Shlwapi.lib")
#endif
#ifdef IS_APPLE
#include <CoreFoundation/CoreFoundation.h>
#endif
#ifdef IS_LINUX
#include <unistd.h>
#endif

#include "util/util.h"

void util::strip_white_space(std::string& str) {
  std::string::iterator end = str.end();
  for(std::string::iterator iter = str.begin(); iter != end;) {
    if(isspace(*iter)) {

      iter = str.erase(iter);
    } else {
      ++iter;
    }
  }
}

double util::string_to_double(const std::string& str) {
  double ret = 0;
  typedef std::string::const_iterator iter;
  iter currChar = str.begin();
  iter end = str.end();
  iter begin = str.begin();
  for(; currChar != end; ++currChar) {
    if(*currChar == '.') break;
  }
  /*Save the currChar position*/
  iter decimal = currChar;
  //currChar points to one past the ones digit
  --currChar;
  double multiplier = 1;
  for(; currChar != begin - 1; --currChar) {
    double charVal = *currChar - '0';
    ret += charVal * multiplier;
    multiplier *= 10;
  }
  if(decimal == end) return ret;

  multiplier = 1.0/10.0;
  for(++decimal; decimal != end; ++decimal) {
    double charVal = *decimal - '0';
    ret += charVal * multiplier;
    multiplier /= 10.0;
  }
  return ret;
}

bool util::is_integer(const std::string& s) {
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p;
   std::strtol(s.c_str(), &p, 10);

   return (*p == 0);
}

long util::string_to_long(const std::string& s) {
  return std::strtol(s.c_str(),nullptr,10);
}

std::string util::read_file(const std::string& path) {
  FILE* file = fopen(path.c_str(), "rb");
  if(!file) {
    throw "No such file or directory: " + path;
  }

  // Seeks to the end of the file to get file size
  fseek(file, 0, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char * buffer = new char[fileSize];
  fread(buffer, sizeof(char), fileSize, file);
  fclose(file);
  std::string result(buffer, buffer + fileSize);
  delete[] buffer;
  return result;
}

bool util::has_only_digits(const std::string& str) {
  return str.find_first_not_of("0123456789") == std::string::npos;
}

// taken from stack overflow
// https://stackoverflow.com/questions/29200635/convert-float-to-string-with-set-precision-number-of-decimal-digits
std::string util::double_to_string(double x, int precision) {
  std::stringstream sstream;
  sstream << std::fixed << std::setprecision(precision) << x;
  return sstream.str();
}

std::string util::resourceRootPath = "";

// Returns the root directory for resources. This directory depends on
// wetherh the program is installed as a bundle or not, and what os we
// are running
std::string util::get_resource_root() {
  if (resourceRootPath != std::string("")) {
    return resourceRootPath;
  }
#ifdef IS_LINUX
  char path[PATH_MAX];
  ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
  assert(len != -1);
  path[len] = 0;
  std::string binDir("/usr/bin");
  if(std::string(path).substr(0, binDir.size()) == binDir) {
#else
  const char* env = std::getenv("DYNSOLVER_RUN_INSTALLED_EXE");
  if (env != nullptr && std::string(env) == std::string("TRUE")) {
#endif
    std::cout << "USING INSTALLATION!" << std::endl;
#ifdef IS_WINDOWS
    HMODULE hModule = GetModuleHandleW(NULL);
    WCHAR pathCstr[MAX_PATH];
    GetModuleFileNameW(hModule, pathCstr, MAX_PATH);
    std::wstring pathWide(pathCstr);
    std::string path(pathWide.begin(), pathWide.end());

	TCHAR buffer[MAX_PATH] = { 0 };
	TCHAR * name;
	DWORD bufSize = sizeof(buffer) / sizeof(*buffer);
	// Get the fully-qualified path of the executable
	if (GetModuleFileName(NULL, buffer, bufSize) == bufSize)
	{
		// the buffer is too small, handle the error somehow
	}
	name = PathFindFileName(buffer);
    // We remove the trailing "/bin/dynsolver.exe" from this string
    path = path.substr(0, path.size() - (std::string("/bin/") + std::string(name)).size());
	std::cout << path << std::endl;
    resourceRootPath = path;
    return resourceRootPath;
#elif IS_APPLE
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE,
                                          (UInt8 *)path, PATH_MAX)) {
      assert(false);
    }
    CFRelease(resourcesURL);
    resourceRootPath = std::string(path);
    return resourceRootPath;
#elif IS_LINUX
    resourceRootPath = std::string("/usr/share/") + std::string(EXE_NAME);
    return resourceRootPath;
#else
    assert(false);
#endif
  } else {
	  std::cout << "USING SOURCE!" << std::endl;
	  resourceRootPath = RESOURCE_ROOT_PATH;
	  return resourceRootPath;
  }
}

std::string util::getPath3dVertexShaderFilePath() {
	return get_resource_root() + "/gl/path_3d.vert";
}

std::string util::getPath3dFragmentShaderFilePath() {
	return get_resource_root() + "/gl/path_3d.frag";
}

std::string util::get2dVertexShaderFilePath() {
	return get_resource_root() + "/gl/2d_renderer.vert";
}

std::string util::get2dFragmentShaderFilePath() {
	return get_resource_root() + "/gl/2d_renderer.frag";
}

std::string util::getDefaultFontPath() {
	return get_resource_root() + "/fonts/OpenSans-Regular.ttf";
}

std::string util::getTextRendererFragmentShaderPath() {
	return get_resource_root() + "/gl/text_renderer.frag";
}

std::string util::getTextRendererVertexShaderPath() {
	return get_resource_root() + "/gl/text_renderer.vert";
}
