cmake_minimum_required(VERSION 3.9.6)
project(regex)

set(SRC_FILES dfa.cpp lexer_builder.cpp lexer.cpp nfa.cpp regex.cpp)
set(PUBLIC_HEADER_FILES lexer_builder.h lexer.h malformed_pattern_exception.h
  nfa.h regex.h)
set(PRIVATE_HEADER_FILES common.h dfa.h)
set(TEST_FILES regex_test.cpp)
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/${PROJECT_NAME}"
  "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/${PROJECT_NAME}"
  "${PRIVATE_HEADER_FILES}")
PREPEND(TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test" "${TEST_FILES}")
add_library(${PROJECT_NAME} STATIC
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}")
target_include_directories(${PROJECT_NAME} PUBLIC include/ PRIVATE src/include)
if ( UNIX )
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")
endif()
if ( WIN32 )
    target_compile_options(${PROJECT_NAME} PRIVATE "/W4")
endif()

add_executable(regex_test EXCLUDE_FROM_ALL
  "${TEST_FILES}")
target_include_directories(regex_test PRIVATE src/include)
target_link_libraries(regex_test PRIVATE Catch2 regex)
add_dependencies(build_tests regex_test)

include(ParseAndAddCatchTests)
ParseAndAddCatchTests(regex_test)

if(DOCS_ENABLED)
  set(DOXYGEN_STRIP_FROM_INC_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include")
  set(DOXYGEN_GENERATE_TAGFILE "${CMAKE_CURRENT_BINARY_DIR}/regex.tag")
  doxygen_add_docs(regex_doc
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include")
  add_dependencies(doc regex_doc)
endif()
