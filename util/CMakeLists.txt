cmake_minimum_required(VERSION 3.9.6)
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
target_compile_definitions(${PROJECT_NAME} PRIVATE EXE_NAME="${EXE_NAME}")
if(UNIX AND NOT APPLE)
  target_compile_definitions(${PROJECT_NAME} PRIVATE IS_LINUX)
endif()
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE IS_WINDOWS)
endif()
if(APPLE)
  target_compile_definitions(${PROJECT_NAME} PRIVATE IS_APPLE)
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_ROOT_PATH="${CMAKE_SOURCE_DIR}/resources")
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
  set(DOXYGEN_GENERATE_TAGFILE "${CMAKE_CURRENT_BINARY_DIR}/util.tag")
  doxygen_add_docs(util_doc
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include")
  add_dependencies(doc util_doc)
endif()
