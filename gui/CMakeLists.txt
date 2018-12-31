cmake_minimum_required(VERSION 3.9.6)
project(gui)
set(SRC_FILES app.cpp
  isocline_dialog.cpp
  saddle_node_bifurcation_dialog.cpp
  console_frame.cpp
  limit_cycle_bifurcation_dialog.cpp
  separatrix_dialog.cpp
  dynamical_dialog.cpp
  model.cpp
  singular_point_dialog.cpp
  dynamical_frame.cpp
  parameter_dialog.cpp
  solution_dialog.cpp
  parameter_frame.cpp
  hopf_bifurcation_dialog.cpp
  saddle_connection_bifurcation_dialog.cpp)
set(PUBLIC_HEADER_FILES app.h common.h)
set(PRIVATE_HEADER_FILES
  isocline_dialog.h
  saddle_node_bifurcation_dialog.h
  console_frame.h
  limit_cycle_bifurcation_dialog.h
  separatrix_dialog.h
  dynamical_dialog.h
  model.h
  singular_point_dialog.h
  dynamical_frame.h
  parameter_dialog.h
  solution_dialog.h
  parameter_frame.h
  hopf_bifurcation_dialog.h
  saddle_connection_bifurcation_dialog.h)
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/${PROJECT_NAME}"
  "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/${PROJECT_NAME}"
  "${PRIVATE_HEADER_FILES}")
set(SRC_FILES ${SRC_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/resources/include/gui/generated.cpp")
set(PRIVATE_HEADER_FILES ${PRIVATE_HEADER_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/resources/include/gui/generated.h")
add_library(${PROJECT_NAME} STATIC
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}")
target_include_directories(${PROJECT_NAME} PUBLIC include/
  PRIVATE src/include resources/include)
target_link_libraries(${PROJECT_NAME} PUBLIC ${WX_WIDGETS_TARGETS}
  PRIVATE OpenGL::GL compiler math mygl regex util parser glfw PUBLIC glad)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  GL_VERSION_MAJOR=${GL_VERSION_MAJOR})
target_compile_definitions(${PROJECT_NAME} PRIVATE
  GL_VERSION_MINOR=${GL_VERSION_MINOR})
if(${GL_VERSION_MAJOR} GREATER 3 AND ${GL_VERSION_MINOR} GREATER 2)
  # Version larger than 4.3
  target_compile_definitions(${PROJECT_NAME} PRIVATE
    GL_ERROR_CALLBACK)
  target_compile_definitions(${PROJECT_NAME} PRIVATE
    GL_ATTRIB_FORMAT)
endif()
if(${USE_GLFW})
target_compile_definitions(${PROJECT_NAME} PUBLIC USE_GLFW)
endif()
if(UNIX AND NOT APPLE)
  target_compile_definitions(${PROJECT_NAME} PUBLIC IS_LINUX)
endif()
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PUBLIC IS_WINDOWS)
endif()
if(APPLE)
  target_compile_definitions(${PROJECT_NAME} PUBLIC IS_APPLE)
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
    "${CMAKE_BINARY_DIR}/compiler/compiler.tag=${CMAKE_BINARY_DIR}/compiler/html"
    "${CMAKE_BINARY_DIR}/parser/parser.tag=${CMAKE_BINARY_DIR}/parser/html"
    "${CMAKE_BINARY_DIR}/util/util.tag=${CMAKE_BINARY_DIR}/util/html"
    "${CMAKE_BINARY_DIR}/regex/regex.tag=${CMAKE_BINARY_DIR}/regex/html"
    "${CMAKE_BINARY_DIR}/gl/gl.tag=${CMAKE_BINARY_DIR}/gl/html"
    "${CMAKE_BINARY_DIR}/math/math.tag=${CMAKE_BINARY_DIR}/math/html")
  set(DOXYGEN_GENERATE_TAGFILE "${CMAKE_CURRENT_BINARY_DIR}/gui.tag")
  doxygen_add_docs(gui_doc
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include")
  add_dependencies(gui_doc compiler_doc regex_doc util_doc parser_doc gl_doc
    math_doc)
  add_dependencies(doc gui_doc)
endif()
