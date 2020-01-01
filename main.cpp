/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>

#include "util/Logger.hpp"
#include "vk/Vulkan.hpp"
#include "vk/builder/VulkanBuilder.hpp"
#include "vk/builder/VertexBufferBuilder.hpp"
#include "vk/builder/GraphicsPipelineBuilder.hpp"
#include "vk/builder/RenderPassBuilder.hpp"
#include "vk/Util.hpp"
#include "vk/RenderPass.hpp"
#include "taiju/shaders/vert/Triangle.hpp"
#include "taiju/shaders/frag/Triangle.hpp"
#include "vk/buffer/VertexBuffer.hpp"
#include "vk/CommandPool.hpp"
#include "vk/CommandBuffer.hpp"

static int _main(util::Logger& log);
static int _mainLoop(util::Logger& log, const std::shared_ptr<vk::Vulkan>& vulkan);

int main() {
  util::Logger log;

  log.setLevel(util::Logger::DEBUG);
  try {
    return _main(log);
  } catch (std::exception& e) {
    fprintf(stderr, "Unhandled exception: \"%s\"\n", e.what());
    return -255;
  } catch(...) {
    fprintf(stderr, "Unknown exception!\n");
    return -255;
  }
}

static int _main(util::Logger& log) {
  GLFWwindow *window;
  // Initialise GLFW
  if (!glfwInit()) {
    log.fatal("Failed to initialize GLFW");
    return -1;
  }
  log.debug("GLFW Initialized.");

  if (!glfwVulkanSupported()) {
    log.fatal("Vulkan not supported");
    return -1;
  }
  log.debug("Vulkan is supported.");

  {
    std::shared_ptr<vk::Vulkan> vulkan = vk::VulkanBuilder(log, "YorabaTaiju", 1920, 1080).build();
    try {
      _mainLoop(log, vulkan);
    } catch (std::exception& e) {
      log.error(e.what());
    } catch(...) {
      log.error("Got unknown error!");
    }
    vulkan->destroy();
  }

  glfwTerminate();
  return 0;
}

static int _mainLoop(util::Logger& log, std::shared_ptr<vk::Vulkan> const& vulkan) {
  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(vulkan->window(), GLFW_STICKY_KEYS, GL_TRUE);

  // FIXME: コンパイルが通るのを調べるだけ。
  auto cmdPool = vulkan->createCommandPool();
  auto cmdBuffer = cmdPool->createBuffer();

  auto renderPassBuilder = vk::RenderPassBuilder(vulkan);
  renderPassBuilder.addSubPass().addColor(0);
  renderPassBuilder.addAttachment(VK_FORMAT_B8G8R8A8_UNORM);

  auto vert = vulkan->createShader<taiju::shaders::vert::Triangle>();
  auto frag = vulkan->createShader<taiju::shaders::frag::Triangle>();
  auto gfxPipeline = vk::GraphicsPipelineBuilder(vulkan, renderPassBuilder.build())
                       .addVertexStage(vert)
                       .addFragmentStage(frag).build();

  std::vector<taiju::shaders::vert::Triangle::Input> vertInputs = {
      {
          .pos = {0.0f, -0.5f},
          .color = {1.0f, 0.0f, 0.0f},
      },
      {
          .pos = {0.5f, 0.5f},
          .color = {0.0f, 1.0f, 0.0f},
      },
      {
          .pos = {-0.5f, 0.5f},
          .color = {0.0f, 0.0f, 1.0f},
      },
  };

  auto vertBuffer = vk::VertexBufferBuilder(vulkan, vertInputs.size() * sizeof(taiju::shaders::vert::Triangle::Input)).build();
  vertBuffer.update(cmdBuffer, vertInputs);

  do {
    //

    // Swap buffers
    glfwSwapBuffers(vulkan->window());
    glfwPollEvents();
  } // Check if the ESC key was pressed or the window was closed
  while (glfwGetKey(vulkan->window(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         glfwWindowShouldClose(vulkan->window()) == 0);

  return 0;
}

