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
#include "shader/Shader.hpp"

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
  // logical device
  uint32_t graphicsQueueFamilyIndex_{};
  uint32_t presentQueueFamilyIndex_{};
  VkDevice vkDevice_{};
  VkQueue vkGraphicsQueue_{};
  VkQueue vkPresentQueue_{};
  // swapchan
  VkSwapchainKHR vkSwapchain_{};
  VkSurfaceFormatKHR vkSwapchainFormat_{};
  std::vector<std::shared_ptr<SwapchainImage>> swapchainImages_{};
  std::vector<std::shared_ptr<Framebuffer>> frameBuffers_{};

private:
  VkDebugReportCallbackEXT vkDebugReportCallback_{};
  PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback_{};

public:
  explicit Vulkan(util::Logger& log);
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

  [[ nodiscard ]] VkPhysicalDeviceMemoryProperties const& vkPhysicalDeviceMemoryProperties() const {
    return this->vkPhysicalDeviceMemoryProperties_;
  }

  [[ nodiscard ]] VkInstance vkInstance() {
    return this->vkInstance_;
  }

  [[ nodiscard ]] VkDevice vkDevice() {
    return this->vkDevice_;
  }

  [[ nodiscard ]] util::Logger& log() {
    return this->log_;
  }

  [[ nodiscard ]] std::shared_ptr<Vulkan> self() {
    return this->shared_from_this();
  }

  [[ nodiscard ]] uint32_t width() const {
    return this->width_;
  }

  [[ nodiscard ]] uint32_t height() const {
    return this->height_;
  }

  [[ nodiscard ]] std::vector<std::shared_ptr<SwapchainImage>>& swapchainImages() {
    return this->swapchainImages_;
  }

  [[ nodiscard ]] VkQueue vkGraphicsQueue() {
    return this->vkGraphicsQueue_;
  }

  [[ nodiscard ]] VkQueue vkPresentQueue() {
    return this->vkPresentQueue_;
  }

public:
  void destroy();

public:
  std::shared_ptr<CommandPool> createCommandPool();

  template<typename T, typename... Args>
  std::shared_ptr<T> createShader(Args &&... args) {
    static_assert(std::is_base_of<Shader, T>::value);
    std::tuple<size_t, const uint32_t*> const binary = Shader::loadBianry<T>();
    std::shared_ptr<Vulkan> vulkan = self();
    ShaderModule module = ShaderModule::create(vulkan, std::get<1>(binary), std::get<0>(binary), typeid(T).name());
    return std::make_shared<T>(vulkan, std::move(module), std::forward<Args>(args)...);
  }

};

}
