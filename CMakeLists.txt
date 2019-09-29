cmake_minimum_required(VERSION 3.14)
project(wakaba)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (UNIX)
  link_libraries(stdc++fs)
endif (UNIX)

###############################################################################
# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})
# GLEW
add_subdirectory(external/glew-2.1.0/build/cmake)
include_directories(external/glew-2.1.0/include)
link_libraries(glew_s)
# GLFW
add_subdirectory(external/glfw)
include_directories(external/glfw/include)
link_libraries(glfw ${GLFW_LIBRARIES})
# GLM
add_subdirectory(external/glm)
include_directories(external/glm)
###############################################################################
# TinyFormat
include_directories(external/tinyformat)
###############################################################################

add_executable(wakaba
    main.cpp
    util/Shared.hpp
    util/Logger.cpp
    util/Logger.hpp
    util/File.hpp
    util/File.cpp
    donut/runtime/Value.hpp
    donut/runtime/Value.cpp
    taiju/Taiju.cpp
    taiju/Taiju.hpp
    gl/Util.hpp
    gl/Util.cpp
    gl/Error.cpp
    gl/Error.hpp
    gl/texture/Texture2D.cpp
    gl/texture/Texture2D.hpp
    gl/texture/TextureUnit.cpp
    gl/texture/TextureUnit.hpp
    gl/program/Program.cpp
    gl/program/Program.hpp
    gl/program/FragmentShader.cpp
    gl/program/FragmentShader.hpp
    gl/program/VertexShader.cpp
    gl/program/VertexShader.hpp
    gl/program/Shader.cpp
    gl/program/Shader.hpp
    gl/vertex/ArrayBuffer.cpp
    gl/vertex/ArrayBuffer.hpp
    gl/vertex/IndexBuffer.cpp
    gl/vertex/IndexBuffer.hpp
    gl/DrawContext.cpp
    gl/DrawContext.hpp
    gl/vertex/VertexArray.cpp
    gl/vertex/VertexArray.hpp)
