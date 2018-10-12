cmake_minimum_required(VERSION 3.12)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif()
project(dynsolver)
set(CMAKE_CXX_STANDARD 11)
find_package(Git REQUIRED)
find_package(PythonInterp REQUIRED)

# Prepends a string to each element in a list
FUNCTION(PREPEND var prefix)
  SET(listVar "")
  FOREACH(f ${ARGN})
    LIST(APPEND listVar "${prefix}/${f}")
  ENDFOREACH(f)
  SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)

set(GL_VERSION 4.3)
find_package(OpenGL ${GL_VERSION} REQUIRED)

# The list of wx widgets required modules.
set(WX_WIDGETS_MODULES core base gl adv propgrid)
set(WX_WIDGETS_VERSION 3.0)

find_package(wxWidgets ${WX_WIDGETS_VERSION} COMPONENTS ${WX_WIDGETS_MODULES})
if(wxWidgets_FOUND)
  include("${wxWidgets_USE_FILE}")
  set(WX_WIDGETS_TARGETS "${wxWidgets_LIBRARIES}")
else()
  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive external/wxWidgets
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  set(wxBUILD_SHARED OFF)
  add_subdirectory(external/wxWidgets)
  set(WX_WIDGETS_TARGETS "${WX_WIDGETS_MODULES}")
endif()

find_package(Freetype)
if(Freetype_FOUND)
  set(FREETYPE_TARGET Freetype::Freetype)
else()
  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive external/freetype2
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  set(BUILD_SHARED_LIBS OFF)
  set(SKIP_INSTALL_ALL ON)
  add_subdirectory(external/freetype2)
  target_include_directories(freetype PUBLIC external/freetype2/include)
  set(FREETYPE_TARGET freetype)
endif()

# Generate glad files.
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive external/glad
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set(GLAD_SRC_FILE  "${CMAKE_BINARY_DIR}/glad/src/glad.c")
set(GLAD_INCLUDE_FILE  "${CMAKE_BINARY_DIR}/glad/include/glad/glad.h")
set(GLAD_INCLUDE_DIR "${CMAKE_BINARY_DIR}/glad/include")
execute_process(
  COMMAND ${PYTHON_EXECUTABLE} -m glad --out-path "${CMAKE_BINARY_DIR}/glad"
  --generator c --spec gl --omit-khrplatform --api gl=${GL_VERSION} --extensions ""
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/external/glad")
if(UNIX)
  set(GLAD_LIBS "${CMAKE_DL_LIBS}")
endif()

if(NOT UNIX)
  set(CMAKE_CXX_FLAGS "/EHa")
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_subdirectory(gui)
add_subdirectory(regex)
add_subdirectory(parser)
add_subdirectory(gl)
add_subdirectory(math)
add_subdirectory(asm)
add_subdirectory(compiler)
add_subdirectory(util)
add_subdirectory(app)
