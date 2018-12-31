cmake_minimum_required(VERSION 3.9.6)
project(compiler)
file(GLOB_RECURSE SRC_FILES
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE PUBLIC_HEADER_FILES
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/*.h")
file(GLOB_RECURSE PRIVATE_HEADER_FILES
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src/include/${PROJECT_NAME}"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/include/${PROJECT_NAME}/*.h")
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/${PROJECT_NAME}"
  "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/${PROJECT_NAME}"
  "${PRIVATE_HEADER_FILES}")
add_library(${PROJECT_NAME} STATIC
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}")
target_include_directories(${PROJECT_NAME} PUBLIC include/
  PRIVATE src/include)
target_link_libraries(${PROJECT_NAME} PRIVATE asm parser util)
if(UNIX)
  target_compile_definitions(${PROJECT_NAME} PUBLIC IS_UNIX)
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC IS_WINDOWS)
endif()
if ( UNIX )
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")
endif()
if ( WIN32 )
    target_compile_options(${PROJECT_NAME} PRIVATE "/W4")
endif()

if(DOCS_ENABLED)
  set(DOXYGEN_STRIP_FROM_INC_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include")
  set(DOXYGEN_TAGFILES
    "${CMAKE_BINARY_DIR}/parser/parser.tag=${CMAKE_BINARY_DIR}/parser/html"
    "${CMAKE_BINARY_DIR}/util/util.tag=${CMAKE_BINARY_DIR}/util/html"
    "${CMAKE_BINARY_DIR}/asm/asm.tag=${CMAKE_BINARY_DIR}/asm/html")
  set(DOXYGEN_GENERATE_TAGFILE "${CMAKE_CURRENT_BINARY_DIR}/compiler.tag")
  doxygen_add_docs(compiler_doc
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include")
  add_dependencies(compiler_doc util_doc asm_doc parser_doc)
  add_dependencies(doc compiler_doc)
endif()
