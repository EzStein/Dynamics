cmake_minimum_required(VERSION 3.12)
project(gl)

find_package(OpenGL REQUIRED)
if(UNIX)
  set(EXTRA_LIBS "${CMAKE_DL_LIBS}")
endif()
execute_process(COMMAND git submodule update --init external/glad
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
execute_process(COMMAND python -m glad --out-path "${CMAKE_CURRENT_BINARY_DIR}"
  --generator c --spec gl --omit-khrplatform --api gl=4.3 --extensions ""
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/external/glad")
set(SRC_FILES buffer.cpp sampler.cpp vertex_array.cpp
  program.cpp shader.cpp texture.cpp)
set(PUBLIC_HEADER_FILES buffer.h program.h sampler.h shader.h
  texture.h vertex_array.h)
set(PRIVATE_HEADER_FILES)
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/${PROJECT_NAME}"
  "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/${PROJECT_NAME}"
  "${PRIVATE_HEADER_FILES}")
add_library(${PROJECT_NAME}
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}"
  "${CMAKE_CURRENT_BINARY_DIR}/src/glad.c")
target_include_directories(${PROJECT_NAME} PUBLIC include/ PRIVATE src/include
  "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL ${EXTRA_LIBS})
