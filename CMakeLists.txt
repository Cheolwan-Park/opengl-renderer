cmake_minimum_required(VERSION 3.16)
project(opengl_rendering_study)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
message(STATUS "module path: ${CMAKE_MODULE_PATH}")

set(CMAKE_CXX_STANDARD 17)
add_compile_options( -Os )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -Os" )
if( MSVC )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17" )
    set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DEBUG" )
else()
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17" )
    set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g" )
endif()

add_executable(
        renderer
        src/main.cpp
)

# sdl
find_package( SDL2 REQUIRED )
find_package( SDL2_image REQUIRED )
include_directories(
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
)
target_link_libraries( renderer
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
)

# opengl
find_package ( OpenGL REQUIRED )
if (OPENGL_FOUND )
    target_link_libraries ( renderer ${OPENGL_LIBRARY} )
    target_link_libraries ( renderer ${OPENGiL_gl_LIBRARY} )
    target_link_libraries ( renderer ${OPENGL_glu_LIBRARY} )
endif()

# glm
find_package( GLM REQUIRED )
if( GLM_FOUND )
    include_directories( ${GLM_INCLUDE_DIRS} )
endif()

# glew
find_package( GLEW REQUIRED )
if( GLEW_FOUND )
    include_directories( ${GLEW_INCLUDE_DIR} )
    target_link_libraries( renderer ${GLEW_LIBRARY} )
endif()

# imgui
add_library(
        imgui
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_widgets.cpp
        imgui/examples/imgui_impl_opengl3.cpp
        imgui/examples/imgui_impl_sdl.cpp
)
target_link_libraries(renderer imgui)
include_directories( imgui/ )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_IMPL_OPENGL_LOADER_GLEW" )