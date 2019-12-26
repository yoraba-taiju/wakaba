cmake_minimum_required(VERSION 3.13)
project(wakaba)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (UNIX)
  link_libraries(stdc++fs)
endif (UNIX)
if (WIN32)
  # https://stackoverflow.com/questions/47690822/possible-to-force-cmake-msvc-to-use-utf-8-encoding-for-source-files-without-a-bo
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif(WIN32)
###############################################################################
# Vulkan
find_package(Vulkan REQUIRED)
include_directories(Vulkan::Vulkan)
link_libraries(Vulkan::Vulkan)
# GLFW
set(GLFW_VULKAN_STATIC ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)
# GLM
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glm)
###############################################################################
# fmt
set(FMT_TEST OFF CACHE BOOL "" FORCE)
set(FMT_FUZZ OFF CACHE BOOL "" FORCE)
add_subdirectory(external/fmt)
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
    vk/GraphicsPipeline.cpp
    vk/GraphicsPipeline.hpp
    vk/Pipeline.cpp
    vk/Pipeline.hpp
    vk/GraphicsPipelineBuilder.cpp
    vk/GraphicsPipelineBuilder.hpp)

target_link_libraries(wakaba fmt::fmt)
target_link_libraries(wakaba glm_static)
target_link_libraries(wakaba glfw)
