/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../util/Logger.hpp"

#include "ShaderModule.hpp"
#include "Device.hpp"

namespace vk {

class VulkanBuilder;
class Framebuffer;
class ShaderModule;
class PipelineLayout;
class SwapchainImage;
class GraphicsPipelineBuilder;
class RenderPassBuilder;
class DeviceMemoryBuilder;
class BufferBuilder;
class CommandPool;

class Vulkan final : public std::enable_shared_from_this<Vulkan> {
private:
  friend class VulkanBuilder;

private: /* Util */
  util::Logger& log_;

private: /* Vulkan */
  // physical device
  GLFWwindow* window_{};
  VkInstance vkInstance_{};
  VkSurfaceKHR vkSurface_{};
  uint32_t width_{};
  uint32_t height_{};
  VkPhysicalDevice vkPhysicalDevice_{};
  VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties_{};

  uint32_t vkGraphicsQueueFamilyIndex_{};
  uint32_t vkPresentQueueFamilyIndex_{};

private:
  VkDebugReportCallbackEXT vkDebugReportCallback_{};
  PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback_{};

public:
  explicit Vulkan(util::Logger& log);
  std::shared_ptr<Vulkan> create(util::Logger& log);
  Vulkan() = delete;
  Vulkan(Vulkan const&) = delete;
  Vulkan(Vulkan&&) = delete;
  Vulkan& operator=(Vulkan const&) = delete;
  Vulkan& operator=(Vulkan&&) = delete;
  ~Vulkan();

public:
  [[ nodiscard ]] GLFWwindow* window() {
    return this->window_;
  }
  [[ nodiscard ]] VkPhysicalDevice vkPhysicalDevice() {
    return this->vkPhysicalDevice_;
  }
  [[ nodiscard ]] VkPhysicalDeviceMemoryProperties const& vkPhysicalDeviceMemoryProperties() const {
    return this->vkPhysicalDeviceMemoryProperties_;
  }
  [[ nodiscard ]] VkSurfaceKHR vkSurface() {
    return this->vkSurface_;
  }
  [[ nodiscard ]] util::Logger& log() {
    return this->log_;
  }
  [[ nodiscard ]] uint32_t width() const {
    return this->width_;
  }
  [[ nodiscard ]] uint32_t height() const {
    return this->height_;
  }
  [[ nodiscard ]] uint32_t vkGraphicsQueueFamilyIndex() const {
    return this->vkGraphicsQueueFamilyIndex_;
  }
  [[ nodiscard ]] uint32_t vkPresentQueueFamilyIndex() const {
    return this->vkPresentQueueFamilyIndex_;
  }
  [[ nodiscard ]] std::shared_ptr<Device> createDevice();
};

}
