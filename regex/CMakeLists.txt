cmake_minimum_required(VERSION 3.12)
project(regex)

set(SRC_FILES dfa.cpp lexer_builder.cpp lexer.cpp nfa.cpp regex.cpp)
set(PUBLIC_HEADER_FILES lexer_builder.h lexer.h malformed_pattern_exception.h
  nfa.h regex.h)
set(PRIVATE_HEADER_FILES common.h dfa.h)
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/${PROJECT_NAME}"
  "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/${PROJECT_NAME}"
  "${PRIVATE_HEADER_FILES}")
add_library(${PROJECT_NAME}
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}")
target_include_directories(${PROJECT_NAME} PUBLIC include/ PRIVATE src/include)
