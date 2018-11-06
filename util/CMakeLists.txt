cmake_minimum_required(VERSION 3.10)
project(util)

set(SRC_FILES util.cpp)
set(PUBLIC_HEADER_FILES util.h)
set(PRIVATE_HEADER_FILES)
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/${PROJECT_NAME}"
  "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/${PROJECT_NAME}"
  "${PRIVATE_HEADER_FILES}")
add_library(${PROJECT_NAME} STATIC
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}")
target_include_directories(${PROJECT_NAME} PUBLIC include/ PRIVATE src/include)
if(UNIX AND NOT APPLE)
  target_compile_definitions(${PROJECT_NAME} PUBLIC IS_LINUX)
endif()
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PUBLIC IS_WINDOWS)
endif()
if(APPLE)
  target_compile_definitions(${PROJECT_NAME} PUBLIC IS_APPLE)
endif()
target_compile_definitions(${PROJECT_NAME} PUBLIC RESOURCE_ROOT_PATH="${CMAKE_SOURCE_DIR}/resources")
