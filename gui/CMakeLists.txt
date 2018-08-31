cmake_minimum_required(VERSION 3.12)
project(gui)
set(SRC_FILES app.cpp
  constants.cpp
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
  generated.cpp
  parameter_frame.cpp
  hopf_bifurcation_dialog.cpp
  saddle_connection_bifurcation_dialog.cpp)
set(PUBLIC_HEADER_FILES app.h common.h)
set(PRIVATE_HEADER_FILES
  constants.h
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
  generated.h
  parameter_frame.h
  hopf_bifurcation_dialog.h
  saddle_connection_bifurcation_dialog.h)
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/${PROJECT_NAME}"
  "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/${PROJECT_NAME}"
  "${PRIVATE_HEADER_FILES}")
set(DEFAULT_FONT_PATH "${CMAKE_SOURCE_DIR}/resources/fonts/OpenSans-Regular.ttf")
set(2D_VERTEX_SHADER_PATH "${CMAKE_SOURCE_DIR}/resources/gl/2d_renderer.vert")
set(2D_FRAGMENT_SHADER_PATH "${CMAKE_SOURCE_DIR}/resources/gl/2d_renderer.frag")
set(3D_VERTEX_SHADER_PATH "${CMAKE_SOURCE_DIR}/resources/gl/path_3d.vert")
set(3D_FRAGMENT_SHADER_PATH "${CMAKE_SOURCE_DIR}/resources/gl/path_3d.frag")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/gui/config.h"
  )
add_library(${PROJECT_NAME}
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}"
  "${GLAD_SRC_FILE}" "${GLAD_INCLUDE_FILE}")
target_include_directories(${PROJECT_NAME} PUBLIC include/
  PRIVATE src/include src/include/gui "${CMAKE_CURRENT_BINARY_DIR}/include"
  PUBLIC "${GLAD_INCLUDE_DIR}")
target_link_libraries(${PROJECT_NAME} PUBLIC ${WX_WIDGETS_TARGETS}
  PRIVATE OpenGL::GL ${GLAD_LIBS} compiler math mygl regex util parser)
