cmake_minimum_required(VERSION 3.10)
project(math2)

set(SRC_FILES main.cpp integer.cpp)
set(PUBLIC_HEADER_FILES eq.h magma.h semigroup.h monoid.h group.h
  abelian_group.h ring.h commutative_ring.h commutative_monoid.h
  field.h module.h vector_space.h integer.h)
set(PRIVATE_HEADER_FILES)
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/math" "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/math" "${PRIVATE_HEADER_FILES}")
add_executable(${PROJECT_NAME}
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

doxygen_add_docs(math_doc
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
