cmake_minimum_required(VERSION 3.12)
project(mygl)
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
add_library(${PROJECT_NAME} STATIC
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}")
target_include_directories(${PROJECT_NAME} PUBLIC include/
  PRIVATE src/include)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glad
  PUBLIC util ${FREETYPE_TARGET})
target_compile_definitions(${PROJECT_NAME} PRIVATE
  GL_VERSION_MAJOR=${GL_VERSION_MAJOR})
target_compile_definitions(${PROJECT_NAME} PRIVATE
  GL_VERSION_MINOR=${GL_VERSION_MINOR})
if(${GL_VERSION_4_3})
  target_compile_definitions(${PROJECT_NAME} PRIVATE
    GL_VERSION_4_3)
endif()
