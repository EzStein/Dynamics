cmake_minimum_required(VERSION 3.12)
project(app)

set(SRC_FILES app.cpp)
set(PUBLIC_HEADER_FILES)
set(PRIVATE_HEADER_FILES app.h)
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/${PROJECT_NAME}"
  "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/${PROJECT_NAME}"
  "${PRIVATE_HEADER_FILES}")
add_executable(${PROJECT_NAME}
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}")
target_include_directories(${PROJECT_NAME} PRIVATE src/include)
target_link_libraries(${PROJECT_NAME} PRIVATE gui)
