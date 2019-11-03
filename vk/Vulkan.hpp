//
// Created by kaede on 2019/10/29.
//

#pragma once

#include "../util/Logger.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../util/Shared.hpp"

namespace vk {

class VulkanBuilder;

class Vulkan final {
private:
  friend class VulkanBuilder;
  VkInstance instance_;
  VkSurfaceKHR surface_;
  GLFWwindow* window_;
private:
  explicit Vulkan();
public:
  inline GLFWwindow* window() {
    return this->window_;
  }
public:
  ENABLE_SHARED_HELPER
};

}
