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
  util::Logger& log_;
  VkInstance instance_;
  VkSurfaceKHR surface_;
  GLFWwindow* window_;
  VkDevice device_;
  VkCommandPool commandPool_;
  VkFence fence_;
private:
  VkDebugReportCallbackEXT vkDebugReportCallback_{};
  PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback_{};
private:
  explicit Vulkan(util::Logger& log);
  ~Vulkan();
public:
  inline GLFWwindow* window() {
    return this->window_;
  }
  inline util::Logger log() {
    return this->log_;
  }
public:
  ENABLE_SHARED_HELPER
};

}
