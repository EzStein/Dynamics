cmake_minimum_required(VERSION 3.12)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif()
project(dynsolver)
set(DYNSOLVER_VERSION_MAJOR 0)
set(DYNSOLVER_VERSION_MINOR 1)
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

if(APPLE)
  set(GL_VERSION_MAJOR 3)
  set(GL_VERSION_MINOR 3)
endif()
if(NOT APPLE)
  set(GL_VERSION_MAJOR 4)
  set(GL_VERSION_MINOR 3)
  set(GL_VERSION_4_3)
endif()

find_package(OpenGL REQUIRED)

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
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/glad/src/glad.c")
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -m glad
    --out-path "${CMAKE_SOURCE_DIR}/glad"
    --generator c --spec gl --omit-khrplatform
    --api gl=${GL_VERSION_MAJOR}.${GL_VERSION_MINOR} --extensions ""
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/external/glad")
endif()
add_subdirectory(glad)

if(WIN32)
  set(CMAKE_CXX_FLAGS "/EHa")
  set(WINDOWS_GUI_FLAG "WIN32")
endif()


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
add_subdirectory(gui)
add_subdirectory(regex)
add_subdirectory(parser)
add_subdirectory(gl)
add_subdirectory(math)
add_subdirectory(asm)
add_subdirectory(compiler)
add_subdirectory(util)
add_subdirectory(app)
add_subdirectory(debug)

# Setup packaging
if(APPLE)
  set(CPACK_GENERATOR DMG)
elseif(UNIX)
  set(CPACK_GENERATOR DEB)
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "ezsuite")
elseif(WIN32)
  set(CPACK_GENERATOR NSIS WIX)
endif()


include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VERSION_MAJOR ${DYNSOLVER_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${DYNSOLVER_VERSION_MINOR})
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_ALL dynsolver_comp)
include(CPack)

