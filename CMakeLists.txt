cmake_minimum_required(VERSION 3.13)
project(wakaba)

set(CMAKE_CXX_STANDARD 20)
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
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)
# GLM
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glm)
###############################################################################
# fmt
set(FMT_TEST OFF CACHE BOOL "" FORCE)
set(FMT_FUZZ OFF CACHE BOOL "" FORCE)
add_subdirectory(external/fmt)
###############################################################################
# fmt
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
add_subdirectory(external/gtest)
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
    taiju/shaders/Triangle.vert
    taiju/shaders/Triangle.frag
    )

add_library(wakaba STATIC
    ${SHADER_HEADERS}

    # helper
    util/Prop.hpp
    util/Logger.cpp
    util/Logger.hpp
    util/File.hpp
    util/File.cpp

    # vk
    vk/Util.cpp
    vk/Util.hpp
    vk/Vulkan.cpp
    vk/Vulkan.hpp
    vk/Device.cpp
    vk/Device.hpp

    #
    vk/Swapchain.cpp
    vk/Swapchain.hpp

    vk/RenderPass.cpp
    vk/RenderPass.hpp

    vk/Framebuffer.cpp
    vk/Framebuffer.hpp

    # vk helpers
    vk/helper/RenderingDispatcher.cpp
    vk/helper/RenderingDispatcher.hpp
    vk/helper/Bridge.cpp
    vk/helper/Bridge.hpp

    # vk commands
    vk/command/CommandPool.cpp
    vk/command/CommandPool.hpp
    vk/command/CommandBuffer.cpp
    vk/command/CommandBuffer.hpp
    vk/command/PrimaryCommandBuffer.cpp
    vk/command/PrimaryCommandBuffer.hpp
    vk/command/SecondaryCommandBuffer.cpp
    vk/command/SecondaryCommandBuffer.hpp

    # vk pipelines
    vk/PipelineLayout.cpp
    vk/PipelineLayout.hpp

    vk/Pipeline.cpp
    vk/Pipeline.hpp
    vk/GraphicsPipeline.cpp
    vk/GraphicsPipeline.hpp

    # vk images
    vk/image/Image.cpp
    vk/image/Image.hpp
    vk/image/SwapchainImage.cpp
    vk/image/SwapchainImage.hpp

    # vk shaders
    vk/ShaderModule.cpp
    vk/ShaderModule.hpp

    vk/shader/Shader.cpp
    vk/shader/Shader.hpp
    vk/shader/VertexShader.cpp
    vk/shader/VertexShader.hpp
    vk/shader/FragmentShader.cpp
    vk/shader/FragmentShader.hpp

    # vk buffers
    vk/buffer/Buffer.cpp
    vk/buffer/Buffer.hpp
    vk/buffer/DeviceMemory.cpp
    vk/buffer/DeviceMemory.hpp

    vk/buffer/VertexBuffer.cpp
    vk/buffer/VertexBuffer.hpp

    # vk builder
    vk/builder/VulkanBuilder.cpp
    vk/builder/VulkanBuilder.hpp

    vk/builder/DeviceMemoryBuilder.cpp
    vk/builder/DeviceMemoryBuilder.hpp
    vk/builder/BufferBuilder.cpp
    vk/builder/BufferBuilder.hpp

    vk/builder/VertexBufferBuilder.cpp
    vk/builder/VertexBufferBuilder.hpp

    vk/builder/RenderPassBuilder.cpp
    vk/builder/RenderPassBuilder.hpp
    vk/builder/GraphicsPipelineBuilder.cpp
    vk/builder/GraphicsPipelineBuilder.hpp
    vk/builder/FramebufferBuilder.cpp
    vk/builder/FramebufferBuilder.hpp

    vk/builder/RenderingDispatcherBuilder.cpp
    vk/builder/RenderingDispatcherBuilder.hpp

    # donut
    donut/runtime/Optional.hpp
    donut/runtime/SubjectiveTime.hpp
    donut/runtime/Value.hpp

    # taiju
    taiju/Taiju.cpp
    taiju/Taiju.hpp
    taiju/scene/Scene.cpp
    taiju/scene/Scene.hpp
    taiju/scenes/TriangleScene.cpp
    taiju/scenes/TriangleScene.hpp
    taiju/scenes/StageScene.cpp
    taiju/scenes/StageScene.hpp
    taiju/shaders/Triangle.cpp
    taiju/shaders/Triangle.hpp

    ## Taiju
    taiju/stage/Geom.hpp
    taiju/stage/Value.hpp

    taiju/stage/Actor.cpp
    taiju/stage/Actor.hpp
    taiju/stage/Stage.cpp
    taiju/stage/Stage.hpp

    taiju/stage/Scenario.cpp
    taiju/stage/Scenario.hpp
    taiju/stage/Conductor.cpp
    taiju/stage/Conductor.hpp
    taiju/stage/Interact.cpp
    taiju/stage/Interact.hpp

    taiju/stage/witches/Witch.cpp
    taiju/stage/witches/Witch.hpp

    taiju/stage/witches/Sora.cpp
    taiju/stage/witches/Sora.hpp
    taiju/stage/witches/Chitose.cpp
    taiju/stage/witches/Chitose.hpp
    taiju/stage/witches/Momiji.cpp
    taiju/stage/witches/Momiji.hpp
    taiju/stage/witches/Kaede.cpp
    taiju/stage/witches/Kaede.hpp

    ##
    taiju/stage/bullets/Bullet.cpp
    taiju/stage/bullets/Bullet.hpp

    # donut - parser
    donut/parser/Parser.cpp
    donut/parser/Parser.hpp
    donut/parser/Stream.cpp
    donut/parser/Stream.hpp

    # donut - ast
    donut/ast/Node.cpp
    donut/ast/Node.hpp
    donut/ast/Position.cpp
    donut/ast/Position.hpp
    #
)

# Mario Badr | Creating a Header-Only Library with CMake
# http://mariobadr.com/creating-a-header-only-library-with-cmake.html
target_link_libraries(wakaba PUBLIC fmt::fmt)
target_link_libraries(wakaba PUBLIC glm::glm_static)
target_link_libraries(wakaba PUBLIC glfw)
target_link_libraries(wakaba PUBLIC Vulkan::Vulkan)
# for pre-compiled shaders
target_include_directories(wakaba PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(wakaba PUBLIC external/glm)

# entry point
add_executable(wakaba_main main.cpp)
target_link_libraries(wakaba_main PRIVATE wakaba)

# unit tests
add_executable(test_main
    donut/parser/StreamTest.cpp
    donut/runtime/ValueTest.cpp
)
target_link_libraries(test_main PRIVATE wakaba)
target_link_libraries(test_main PRIVATE gtest)
target_link_libraries(test_main PRIVATE gtest_main)
