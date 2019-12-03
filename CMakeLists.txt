cmake_minimum_required(VERSION 3.13)
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
# Vulkan
find_package(Vulkan REQUIRED)
include_directories(Vulkan::Vulkan)
link_libraries(Vulkan::Vulkan)
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
    taiju/scene/Scene.cpp
    taiju/scene/Scene.hpp
    taiju/scenes/TriangleScene.cpp
    taiju/scenes/TriangleScene.hpp
    vk/Util.cpp
    vk/Util.hpp
    vk/Vulkan.cpp
    vk/Vulkan.hpp
    vk/VulkanBuilder.cpp
    vk/VulkanBuilder.hpp
    vk/FrameBuffer.cpp
    vk/FrameBuffer.hpp
    vk/RenderPass.cpp
    vk/RenderPass.hpp
    vk/CommandBuffer.cpp
    vk/CommandBuffer.hpp
    vk/CommandPool.cpp
    vk/CommandPool.hpp
    vk/ShaderModule.cpp
    vk/ShaderModule.hpp
    vk/PipelineLayout.cpp
    vk/PipelineLayout.hpp
    )
