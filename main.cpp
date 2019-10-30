#include <iostream>

#include <GL/glew.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>

#include "util/Logger.hpp"
#include "vk/Vulkan.hpp"
#include "vk/Util.hpp"

static int _main(util::Logger& log);

int main() {
  util::Logger log;

  log.setLevel(util::Logger::DEBUG);
  try {
    return _main(log);
  } catch (std::exception& e) {
    fprintf(stderr, "Unhandled exception: \"%s\"\n", e.what());
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

  for (std::string const& extention : vk::enumurateRequiredInstanceExtensions()) {
    log.debug("Vulkan Required Extention: %s", extention);
  }

  std::vector<VkLayerProperties> props = vk::enumerateInstanceLayerProperties();
  for(VkLayerProperties const& prop : props) {
    log.debug("Layer: %s (spec=%d, impl=%d) :: %s",
        prop.layerName,
        prop.specVersion,
        prop.implementationVersion,
        prop.description);
  }


  std::shared_ptr<vk::Vulkan> vulkan = vk::Vulkan::createInstance(log, "YorabaTaiju");

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(vulkan->window(), GLFW_STICKY_KEYS, GL_TRUE);

  do {

    // Swap buffers
    glfwSwapBuffers(vulkan->window());
    glfwPollEvents();

  } // Check if the ESC key was pressed or the window was closed
  while (glfwGetKey(vulkan->window(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         glfwWindowShouldClose(vulkan->window()) == 0);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();
  return 0;
}

