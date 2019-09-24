cmake_minimum_required(VERSION 3.14)
project(wakaba)

set(CMAKE_CXX_STANDARD 17)

###############################################################################
# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})
# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARY})
# GLFW
add_subdirectory(external/glfw)
include_directories(external/glfw/include)
link_libraries(glfw ${GLFW_LIBRARIES})
###############################################################################
# TinyFormat
include_directories(external/tinyformat)
###############################################################################

add_executable(wakaba
        main.cpp
        util/Logger.cpp
        util/Logger.hpp
        donut/runtime/Value.hpp
        donut/runtime/Value.cpp
        taiju/Taiju.cpp taiju/Taiju.hpp gl/texture/Texture.cpp gl/texture/Texture.hpp gl/texture/TexturePool.cpp gl/texture/TexturePool.hpp gl/texture/TextureUnit.cpp gl/texture/TextureUnit.hpp gl/program/Program.cpp gl/program/Program.hpp gl/program/FragmentShader.cpp gl/program/FragmentShader.hpp gl/program/VertexShader.cpp gl/program/VertexShader.hpp util/SharedUtil.hpp gl/program/Shader.cpp gl/program/Shader.hpp)
