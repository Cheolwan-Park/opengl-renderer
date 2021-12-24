cmake_minimum_required(VERSION 3.16)
project(opengl_rendering_study)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)
message(STATUS "module path: ${CMAKE_MODULE_PATH}")
message(STATUS "sdl path: ${SDL2_PATH}")

set(CMAKE_CXX_STANDARD 20)
add_compile_options( -Os )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -Os" )
if( MSVC )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17" )
    set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DEBUG" )
else()
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17" )
    set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g" )
endif()


add_executable(renderer_test main.cpp)

add_subdirectory(renderer)
target_link_libraries(renderer_test renderer)
target_include_directories(
    renderer_test
    PUBLIC
    ${REND_INCLUDE_DIR}
)

# glm
find_package( GLM REQUIRED )
if( GLM_FOUND )
  target_include_directories(
    renderer
    PUBLIC
    ${GLM_INCLUDE_DIRS} 
  )
endif()