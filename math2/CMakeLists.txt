cmake_minimum_required(VERSION 3.9.6)
project(math2)

set(SRC_FILES main.cpp integer.cpp)
set(PUBLIC_HEADER_FILES eq.h magma.h semigroup.h monoid.h group.h
  abelian_group.h ring.h commutative_ring.h commutative_monoid.h
  field.h module.h vector_space.h integer.h)
set(TEST_FILES main_test.cpp integer_test.cpp)
set(PRIVATE_HEADER_FILES)
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/math" "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/math" "${PRIVATE_HEADER_FILES}")
PREPEND(TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test" "${TEST_FILES}")
add_library(${PROJECT_NAME} STATIC
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}")
target_include_directories(${PROJECT_NAME} PUBLIC include/ PRIVATE src/include)
if ( UNIX )
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wconversion")
endif()
if ( WIN32 )
    target_compile_options(${PROJECT_NAME} PRIVATE "/W4")
endif()

add_executable(math_test EXCLUDE_FROM_ALL "${TEST_FILES}")
target_include_directories(math_test PRIVATE src/include)
target_link_libraries(math_test PRIVATE Catch2 math2)
add_dependencies(build_tests math_test)

include(ParseAndAddCatchTests)
ParseAndAddCatchTests(math_test)

if(DOCS_ENABLED)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE
    "${CMAKE_CURRENT_SOURCE_DIR}/doc/mainpage.md")
  set(DOXYGEN_STRIP_FROM_INC_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include")
  doxygen_add_docs(math2_doc
    "${CMAKE_CURRENT_SOURCE_DIR}/doc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include")
  add_dependencies(doc math2_doc)
endif()
