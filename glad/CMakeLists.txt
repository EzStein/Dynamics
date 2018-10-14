cmake_minimum_required(VERSION 3.12)
project(glad)

add_library(${PROJECT_NAME} STATIC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h")

target_include_directories(${PROJECT_NAME} PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
if(UNIX)
  target_link_libraries(${PROJECT_NAME} PRIVATE "${CMAKE_DL_LIBS}")
endif()
