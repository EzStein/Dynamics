cmake_minimum_required(VERSION 3.12)
project(math)
set(SRC_FILES complex.cpp matrix_3x3.cpp poly_matrix.cpp
  quaternion.cpp vector2d.cpp window2d.cpp eigenvalue.cpp matrix_4x4.cpp
  polynomial.cpp square_matrix.cpp vector3d.cpp window3d.cpp matrix_2x2.cpp
  matrix.cpp poly_root.cpp util.cpp vector.cpp)
set(PUBLIC_HEADER_FILES complex.h matrix_3x3.h poly_matrix.h quaternion.h
  vector2d.h window2d.h eigenvalue.h matrix_4x4.h polynomial.h square_matrix.h
  vector3d.h window3d.h matrix_2x2.h matrix.h poly_root.h util.h vector.h)
set(PRIVATE_HEADER_FILES)
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