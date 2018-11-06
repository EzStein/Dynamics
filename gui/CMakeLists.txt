cmake_minimum_required(VERSION 3.12)
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
  generated.cpp
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
  generated.h
  parameter_frame.h
  hopf_bifurcation_dialog.h
  saddle_connection_bifurcation_dialog.h)
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
  PRIVATE src/include src/include/gui)
target_link_libraries(${PROJECT_NAME} PUBLIC ${WX_WIDGETS_TARGETS} glad
  PRIVATE OpenGL::GL compiler math mygl regex util parser)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  GL_VERSION_MAJOR=${GL_VERSION_MAJOR})
target_compile_definitions(${PROJECT_NAME} PRIVATE
  GL_VERSION_MINOR=${GL_VERSION_MINOR})
if(UNIX AND NOT APPLE)
  target_compile_definitions(${PROJECT_NAME} PUBLIC IS_LINUX)
endif()
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PUBLIC IS_WINDOWS)
endif()
if(APPLE)
  target_compile_definitions(${PROJECT_NAME} PUBLIC IS_APPLE)
endif()