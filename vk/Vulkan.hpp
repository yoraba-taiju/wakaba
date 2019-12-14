//
// Created by kaede on 2019/10/29.
//

#pragma once

#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../util/Logger.hpp"
#include "../util/Shared.hpp"

#include "CommandBuffer.hpp"
#include "ShaderModule.hpp"


namespace vk {

class VulkanBuilder;
class FrameBuffer;
class ShaderModule;
class GraphicsPipeline;
class PipelineLayout;

class Vulkan final : std::enable_shared_from_this<Vulkan> {
private:
  friend class VulkanBuilder;

  util::Logger &log_;
  VkInstance instance_;
  VkSurfaceKHR surface_;
  GLFWwindow* window_;
  VkPhysicalDevice physicalDevice_;
  uint32_t queueFamilyIndex_;
  VkDevice device_;
  VkFence fence_;
  VkSwapchainKHR swapchain_;
  std::vector<VkImage> swapchainImages_;
  std::vector<VkImageView> swapchainImageViews_;
  std::vector<std::shared_ptr<FrameBuffer>> frameBuffers_;
private:
  VkDebugReportCallbackEXT vkDebugReportCallback_;
  PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback_;
private:
  explicit Vulkan(util::Logger &log);
  ~Vulkan();

public:
  inline GLFWwindow* window() {
    return this->window_;
  }

  inline VkInstance instance() {
    return this->instance_;
  }

  inline VkDevice device() {
    return this->device_;
  }

  inline util::Logger log() {
    return this->log_;
  }

  inline std::shared_ptr<Vulkan> self() {
    return this->shared_from_this();
  }

public:
  void destroy();
  std::shared_ptr<ShaderModule> loadShaderFromFile(std::string const& filename);
  std::shared_ptr<GraphicsPipeline> createGraphicsPipeline();

public:
  ENABLE_SHARED_HELPER
};

}
