cmake_minimum_required(VERSION 3.10)
project(app)

set(SRC_FILES app.cpp)
set(PUBLIC_HEADER_FILES)
set(PRIVATE_HEADER_FILES app.h)
PREPEND(SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SRC_FILES}")
PREPEND(PUBLIC_HEADER_FILES "include/${PROJECT_NAME}"
  "${PUBLIC_HEADER_FILES}")
PREPEND(PRIVATE_HEADER_FILES "src/include/${PROJECT_NAME}"
  "${PRIVATE_HEADER_FILES}")
if(WIN32)
  set(SRC_FILES ${SRC_FILES} "${CMAKE_SOURCE_DIR}/resources/windows/app.rc"
"${CMAKE_SOURCE_DIR}/resources/windows/app.manifest")
endif()
add_executable(${PROJECT_NAME} ${EXE_FLAG}
  "${SRC_FILES}"
  "${PUBLIC_HEADER_FILES}"
  "${PRIVATE_HEADER_FILES}")
target_include_directories(${PROJECT_NAME} PRIVATE src/include)
target_link_libraries(${PROJECT_NAME} PRIVATE gui)

# Setup installation
if(APPLE)
install(TARGETS ${PROJECT_NAME} DESTINATION "../MacOS"
  COMPONENT dynsolver_comp)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/resources/gl" DESTINATION "."
  COMPONENT dynsolver_comp)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/resources/fonts" DESTINATION "."
  COMPONENT dynsolver_comp)
elseif(WIN32)
install(TARGETS ${PROJECT_NAME} DESTINATION "bin"
  COMPONENT dynsolver_comp)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/resources/gl" DESTINATION "."
  COMPONENT dynsolver_comp)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/resources/fonts" DESTINATION "."
  COMPONENT dynsolver_comp)
else()
install(TARGETS ${PROJECT_NAME} DESTINATION "bin"
  COMPONENT dynsolver_comp)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/resources/gl" DESTINATION "share/${EXE_NAME}"
  COMPONENT dynsolver_comp)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/resources/fonts" DESTINATION "share/${EXE_NAME}"
  COMPONENT dynsolver_comp)
install(FILES "${CMAKE_SOURCE_DIR}/resources/linux/xyz.ezstein.dynsolver.desktop" DESTINATION "share/applications"
  COMPONENT dynsolver_comp)
install(FILES "${CMAKE_SOURCE_DIR}/resources/linux/icon.png" DESTINATION "share/${EXE_NAME}"
  COMPONENT dynsolver_comp)
endif()

if ( UNIX )
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")
endif()
if ( WIN32 )
    target_compile_options(${PROJECT_NAME} PRIVATE "/W4")
endif()
set_target_properties(app PROPERTIES OUTPUT_NAME "${EXE_NAME}")
