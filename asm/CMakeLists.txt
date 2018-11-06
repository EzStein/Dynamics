cmake_minimum_required(VERSION 3.10)
project(asm)
set(SRC_FILES asm.cpp assembler.cpp)
set(PUBLIC_HEADER_FILES assembler.h asm.h)
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
target_link_libraries(${PROJECT_NAME} PUBLIC regex)
