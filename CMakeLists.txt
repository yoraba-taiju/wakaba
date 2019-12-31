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

# [CMake] Build rule for specific file extension
# https://cmake.org/pipermail/cmake/2010-June/037733.html

function(compileShaders outVariable)
  set(result)
  foreach(input ${ARGN})
    # prepare/set up path for input/output.
    set(input "${CMAKE_CURRENT_SOURCE_DIR}/${input}")
    file(RELATIVE_PATH output ${CMAKE_CURRENT_SOURCE_DIR} ${input})
    set(output "${CMAKE_CURRENT_BINARY_DIR}/${output}.h")
    # create directory for output.
    get_filename_component(outputDir ${output} DIRECTORY)
    file(MAKE_DIRECTORY ${outputDir})
    # compile it.
    add_custom_command(OUTPUT ${output}
        COMMAND glslangValidator -V ${input} --variable-name shaderBinary -o ${output}
        DEPENDS ${input}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Compiling shaders from ${input} -> ${output}"
        VERBATIM
    )
    list(APPEND result ${output})
  endforeach()
  set(${outVariable} "${result}" PARENT_SCOPE)
endfunction()

compileShaders(SHADER_HEADERS
    taiju/shaders/vert/Triangle.vert
    taiju/shaders/frag/Triangle.frag
    )

add_executable(wakaba
    ${SHADER_HEADERS}
    main.cpp
    # util
    util/Shared.hpp
    util/Logger.cpp
    util/Logger.hpp
    util/File.hpp
    util/File.cpp
    # vk
    vk/Util.cpp
    vk/Util.hpp
    vk/Vulkan.cpp
    vk/Vulkan.hpp
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
    vk/image/Image.cpp
    vk/image/Image.hpp
    vk/shader/Shader.cpp
    vk/shader/Shader.hpp
    vk/shader/VertexShader.cpp
    vk/shader/VertexShader.hpp
    vk/shader/FragmentShader.cpp
    vk/shader/FragmentShader.hpp
    # vk builder
    vk/builder/VulkanBuilder.cpp
    vk/builder/VulkanBuilder.hpp
    vk/builder/GraphicsPipelineBuilder.cpp
    vk/builder/GraphicsPipelineBuilder.hpp
    # donut
    donut/runtime/Value.hpp
    donut/runtime/Value.cpp
    # taiju
    taiju/Taiju.cpp
    taiju/Taiju.hpp
    taiju/scene/Scene.cpp
    taiju/scene/Scene.hpp
    taiju/scenes/TriangleScene.cpp
    taiju/scenes/TriangleScene.hpp
    taiju/shaders/vert/Triangle.cpp
    taiju/shaders/vert/Triangle.hpp
    taiju/shaders/frag/Triangle.cpp
    taiju/shaders/frag/Triangle.hpp
    vk/builder/RenderPassBuilder.cpp vk/builder/RenderPassBuilder.hpp vk/image/SwapchainImage.cpp vk/image/SwapchainImage.hpp vk/buffer/Buffer.cpp vk/buffer/Buffer.hpp vk/buffer/DeviceMemory.cpp vk/buffer/DeviceMemory.hpp vk/builder/DeviceMemoryBuilder.cpp vk/builder/DeviceMemoryBuilder.hpp vk/builder/BufferBuilder.cpp vk/builder/BufferBuilder.hpp vk/buffer/HostVisibleDeviceMemory.cpp vk/buffer/HostVisibleDeviceMemory.hpp vk/builder/HostInvisibleDeviceMemoryBuilder.cpp vk/builder/HostInvisibleDeviceMemoryBuilder.hpp)

target_link_libraries(wakaba fmt::fmt)
target_link_libraries(wakaba glm_static)
target_link_libraries(wakaba glfw)
target_link_libraries(wakaba Vulkan::Vulkan)
target_include_directories(wakaba PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(wakaba PRIVATE Vulkan::Vulkan)
target_include_directories(wakaba PRIVATE glfw::glfw)
target_include_directories(wakaba PRIVATE glm::glm)
target_include_directories(wakaba PRIVATE fmt::fmt)

