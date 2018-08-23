cmake_minimum_required(VERSION 3.12)
project(mygl)
find_package(OpenGL 4.3 REQUIRED)
find_package(Freetype REQUIRED)
LOAD_GLAD()
set(SRC_FILES buffer.cpp sampler.cpp vertex_array.cpp
  program.cpp shader.cpp texture.cpp font.cpp text_renderer.cpp
  free_type_library_singleton.cpp)
set(PUBLIC_HEADER_FILES buffer.h program.h sampler.h shader.h
  texture.h vertex_array.h font.h text_renderer.h)
set(PRIVATE_HEADER_FILES free_type_library_singleton.h)
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/gl"
  "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/gl"
  "${PRIVATE_HEADER_FILES}")
add_library(${PROJECT_NAME}
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}"
  "${GLAD_SRC}")
set(VERTEX_SHADER_PATH "${CMAKE_SOURCE_DIR}/resources/gl/text_renderer.vert")
set(FRAGMENT_SHADER_PATH "${CMAKE_SOURCE_DIR}/resources/gl/text_renderer.frag")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/gl/config.h"
  )
target_include_directories(${PROJECT_NAME} PUBLIC include/ PRIVATE src/include
  "${GLAD_INCLUDE}" "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL
   ${GLAD_EXTRA_LIBS} util PUBLIC Freetype::Freetype)
