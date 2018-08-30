cmake_minimum_required(VERSION 3.12)
project(compiler)
file(GLOB_RECURSE SRC_FILES
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE PUBLIC_HEADER_FILES
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/*.h")
file(GLOB_RECURSE PRIVATE_HEADER_FILES
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src/include/${PROJECT_NAME}"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/include/${PROJECT_NAME}/*.h")
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/${PROJECT_NAME}"
  "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/${PROJECT_NAME}"
  "${PRIVATE_HEADER_FILES}")
if(UNIX)
  set(IS_UNIX true)
endif()
if(MSVC)
  set(IS_WINDOWS true)
endif()
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/compiler/config.h"
  )
add_library(${PROJECT_NAME}
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}"
  "${CMAKE_CURRENT_BINARY_DIR}/include/compiler/config.h")
target_include_directories(${PROJECT_NAME} PUBLIC include/ PRIVATE src/include
  PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE asm parser util)
