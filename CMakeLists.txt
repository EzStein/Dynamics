cmake_minimum_required(VERSION 3.10)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif()
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()
project(dynsolver)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)
set(DYNSOLVER_VERSION_MAJOR 0)
set(DYNSOLVER_VERSION_MINOR 1)
find_package(Git REQUIRED)
find_package(PythonInterp REQUIRED)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/test_install")
set(EXE_NAME "dynsolver")

# Prepends a string to each element in a list
FUNCTION(PREPEND var prefix)
  SET(listVar "")
  FOREACH(f ${ARGN})
    LIST(APPEND listVar "${prefix}/${f}")
  ENDFOREACH(f)
  SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)

FUNCTION(get_all_targets _result _dir)
    get_property(_subdirs DIRECTORY "${_dir}" PROPERTY SUBDIRECTORIES)
    foreach(_subdir IN LISTS _subdirs)
        get_all_targets(${_result} "${_subdir}")
    endforeach()
    get_property(_sub_targets DIRECTORY "${_dir}" PROPERTY BUILDSYSTEM_TARGETS)
    set(${_result} ${${_result}} ${_sub_targets} PARENT_SCOPE)
ENDFUNCTION()

if(APPLE)
  set(GL_VERSION_MAJOR 3)
  set(GL_VERSION_MINOR 3)
endif()
if(NOT APPLE)
  set(GL_VERSION_MAJOR 4)
  set(GL_VERSION_MINOR 3)
  set(GL_VERSION_4_3)
endif()

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# The list of wx widgets required modules.
set(WX_WIDGETS_MODULES core base gl adv propgrid)
set(WX_WIDGETS_VERSION 3.0)
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive external/wxWidgets
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set(wxBUILD_SHARED OFF)
set(wxBUILD_PRECOMP OFF)
set(wxUSE_ZLIB OFF)
set(wxUSE_ZIPSTREAM OFF)
set(wxUSE_LIBLZMA OFF)
set(wxUSE_LIBPNG OFF)
set(wxUSE_ARTPROVIDER_TANGO OFF)
set(wxUSE_SVG OFF)
set(wxUSE_LIBJPEG OFF)
set(wxUSE_LIBTIFF OFF)
set(wxUSE_EXPAT OFF)
set(wxUSE_REGEX OFF)
set(wxBUILD_INSTALL OFF)
set(wxUSE_STL ON)

add_subdirectory(external/wxWidgets)

# Exclude wx widgets targets from the all/default target
get_all_targets(wxTargets external/wxWidgets)
foreach(target IN LISTS wxTargets)
  set_property(TARGET ${target} PROPERTY EXCLUDE_FROM_ALL ON)
endforeach()
set(WX_WIDGETS_TARGETS "${WX_WIDGETS_MODULES}")

# Freetype configuration
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive external/freetype2
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set(BUILD_SHARED_LIBS OFF)
set(SKIP_INSTALL_ALL ON)
set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz ON)
set(CMAKE_DISABLE_FIND_PACKAGE_BZip2 ON)
set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB ON)
set(CMAKE_DISABLE_FIND_PACKAGE_PNG ON)
add_subdirectory(external/freetype2)

# Exclude freetype targets from the all/default target
get_all_targets(ftTargets external/freetype2)
foreach(target IN LISTS ftTargets)
  set_property(TARGET ${target} PROPERTY EXCLUDE_FROM_ALL ON)
endforeach()

target_include_directories(freetype PUBLIC external/freetype2/include)
set(FREETYPE_TARGET freetype)


# Generate glad files.
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive external/glad
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -m glad
    --profile core
    --out-path "${CMAKE_SOURCE_DIR}/glad"
    --generator c --spec gl
    --api gl=${GL_VERSION_MAJOR}.${GL_VERSION_MINOR} --extensions ""
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/external/glad")
add_subdirectory(glad)

if(WIN32)
  set(CMAKE_CXX_FLAGS "/EHa")
  set(EXE_FLAG "WIN32")
endif()

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
  set(CPACK_GENERATOR Bundle)
  set(CPACK_BUNDLE_NAME dynsolver)
  set(CPACK_BUNDLE_PLIST "${CMAKE_SOURCE_DIR}/resources/apple/info.plist")
  set(CPACK_BUNDLE_ICON "${CMAKE_SOURCE_DIR}/resources/apple/icon.icns")
elseif(UNIX)
  set(CPACK_GENERATOR DEB RPM)
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "ezsuite")
  set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/share/applications")
elseif(WIN32)
  set(CPACK_GENERATOR NSIS)
  set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/resources/windows/app.ico")
  set(CPACK_NSIS_MODIFY_PATH "ON")
  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON")
  set(CPACK_PACKAGE_EXECUTABLES "${EXE_NAME};${EXE_NAME}")
  set(CPACK_CREATE_DESKTOP_LINKS "${EXE_NAME}")

# Set/unset the environment variable DYNSOLVER_RUN_INSTALLED_EXE to TRUE
  set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
"!include \\\"winmessages.nsh\\\"
 !define env_hklm 'HKLM \\\"SYSTEM\\\\CurrentControlSet\\\\Control\\\\Session Manager\\\\Environment\\\"'
 !define env_hkcu 'HKCU \\\"Environment\\\"'
 WriteRegExpandStr \\\${env_hklm} 'DYNSOLVER_RUN_INSTALLED_EXE' 'TRUE'
 WriteRegExpandStr \\\${env_hkcu} 'DYNSOLVER_RUN_INSTALLED_EXE' 'TRUE'
 SendMessage \\\${HWND_BROADCAST} \\\${WM_WININICHANGE} 0 \\\"STR:Environment\\\" /TIMEOUT=5000")
 set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
"DeleteRegValue \\\${env_hklm} 'DYNSOLVER_RUN_INSTALLED_EXE'
 DeleteRegValue \\\${env_hkcu} 'DYNSOLVER_RUN_INSTALLED_EXE'
 SendMessage \\\${HWND_BROADCAST} \\\${WM_WININICHANGE} 0 \\\"STR:Environment\\\" /TIMEOUT=5000")
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VERSION_MAJOR ${DYNSOLVER_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${DYNSOLVER_VERSION_MINOR})
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};app;dynsolver_comp;/")
include(CPack)
