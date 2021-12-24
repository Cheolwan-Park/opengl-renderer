set(REND_SOURCE_DIR src)
set(REND_SOURCES
    ${REND_SOURCE_DIR}/test-programs/TestProgram.cpp
    ${REND_SOURCE_DIR}/Application.cpp
    ${REND_SOURCE_DIR}/Input.cpp
    ${REND_SOURCE_DIR}/Mesh.cpp
    ${REND_SOURCE_DIR}/Shader.cpp
    ${REND_SOURCE_DIR}/Texture.cpp
)

set(REND_HEADER_DIR include)
set(REND_HEADERS
    ${REND_HEADER_DIR}/test-programs/TestProgram.hpp
    ${REND_HEADER_DIR}/Application.hpp
    ${REND_HEADER_DIR}/Camera.hpp
    ${REND_HEADER_DIR}/Input.hpp
    ${REND_HEADER_DIR}/Mesh.hpp
    ${REND_HEADER_DIR}/Shader.hpp
    ${REND_HEADER_DIR}/Texture.hpp
    ${REND_HEADER_DIR}/util.hpp
)

add_library(renderer STATIC ${REND_HEADERS} ${REND_SOURCES})

set(REND_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${REND_HEADER_DIR}")
target_include_directories(
  renderer
  PUBLIC 
  ${REND_INCLUDE_DIR}
)

# sdl
find_package( SDL2 REQUIRED )
target_include_directories(
  renderer
  PUBLIC
  ${SDL2_INCLUDE_DIR}
)
include_directories(
  ${SDL2_INCLUDE_DIR}
)
target_link_libraries(
  renderer
  ${SDL2_LIBRARY}
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
  target_include_directories(
    renderer
    PUBLIC
    ${GLM_INCLUDE_DIRS} 
  )
endif()

# glew
find_package( GLEW REQUIRED )
if( GLEW_FOUND )
  message(STATUS "GLEW found, INCLUDE_DIR: ${GLEW_INCLUDE_DIR}")
  include_directories(
    renderer
    PUBLIC
    ${GLEW_INCLUDE_DIR} 
  )
  include_directories(
    ${GLEW_INCLUDE_DIR} 
  )
  target_link_libraries(
    renderer 
    ${GLEW_LIBRARY} 
  )
endif()

# imgui
add_library(
  imgui
  ${PROJECT_SOURCE_DIR}/imgui/imgui.cpp
  ${PROJECT_SOURCE_DIR}/imgui/imgui_demo.cpp
  ${PROJECT_SOURCE_DIR}/imgui/imgui_draw.cpp
  ${PROJECT_SOURCE_DIR}/imgui/imgui_widgets.cpp
  ${PROJECT_SOURCE_DIR}/imgui/examples/imgui_impl_opengl3.cpp
  ${PROJECT_SOURCE_DIR}/imgui/examples/imgui_impl_sdl.cpp
)
target_link_libraries(renderer imgui)
include_directories(
  ${PROJECT_SOURCE_DIR}/imgui/ 
)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_IMPL_OPENGL_LOADER_GLEW" )

# OBJ-Loader
target_include_directories(
  renderer
  PUBLIC
  ${PROJECT_SOURCE_DIR}/OBJ-Loader/Source
)

# SOIL2
add_library(
  soil2
  ${PROJECT_SOURCE_DIR}/SOIL2/src/SOIL2/image_DXT.c
  ${PROJECT_SOURCE_DIR}/SOIL2/src/SOIL2/image_helper.c
  ${PROJECT_SOURCE_DIR}/SOIL2/src/SOIL2/SOIL2.c
  ${PROJECT_SOURCE_DIR}/SOIL2/src/SOIL2/wfETC.c
  ${PROJECT_SOURCE_DIR}/SOIL2/src/common/common.cpp
)
target_link_libraries(renderer soil2)
target_include_directories(
  renderer
  PUBLIC
  ${PROJECT_SOURCE_DIR}/SOIL2/src/
)