cmake_minimum_required(VERSION 3.12)
project(parser)
set(SRC_FILES grammar.cpp lr_parser.cpp syntax_exception.cpp)
set(PUBLIC_HEADER_FILES grammar.h lr_parser.h syntax_exception.h common.h)
set(PRIVATE_HEADER_FILES)
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/${PROJECT_NAME}"
  "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/${PROJECT_NAME}"
  "${PRIVATE_HEADER_FILES}")
add_library(${PROJECT_NAME} SHARED
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}")
target_include_directories(${PROJECT_NAME} PUBLIC include/ PRIVATE src/include)
target_link_libraries(${PROJECT_NAME} PUBLIC regex)
