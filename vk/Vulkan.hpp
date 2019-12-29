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

#include "shader/Shader.hpp"

namespace vk {

class VulkanBuilder;
class FrameBuffer;
class ShaderModule;
class PipelineLayout;
class GraphicsPipelineBuilder;

class Vulkan final : public std::enable_shared_from_this<Vulkan> {
private:
  friend class VulkanBuilder;

private: /* Util */
  util::Logger &log_;

private: /* Vulkan */
  VkInstance instance_;
  VkSurfaceKHR surface_;
  GLFWwindow* window_;
  uint32_t width_;
  uint32_t height_;
  VkPhysicalDevice physicalDevice_;
  uint32_t graphicsQueueFamiliIndex_;
  uint32_t presentQueueFamiliIndex_;
  VkDevice device_;
  VkQueue graphicsQueue_;
  VkQueue presentQueue_;
  VkFence fence_;
  VkSwapchainKHR swapchain_;
  VkSurfaceFormatKHR swapchainFormat_;
  std::vector<VkImage> swapchainImages_;
  std::vector<VkImageView> swapchainImageViews_;
  std::vector<std::shared_ptr<FrameBuffer>> frameBuffers_;

private:
  VkDebugReportCallbackEXT vkDebugReportCallback_;
  PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback_;

public:
  explicit Vulkan(util::Logger &log);
  ~Vulkan();

public:
  [[ nodiscard ]] GLFWwindow* window() {
    return this->window_;
  }

  [[ nodiscard ]] VkInstance instance() {
    return this->instance_;
  }

  [[ nodiscard ]] VkDevice device() {
    return this->device_;
  }

  [[ nodiscard ]] util::Logger log() {
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

public:
  void destroy();

public:
  std::shared_ptr<GraphicsPipelineBuilder> createGraphicsPipelineBuilder();

  template<typename T, typename... Args>
  std::shared_ptr<T> createShader(Args &&... args) {
    static_assert(std::is_base_of<Shader, T>::value);
    std::tuple<size_t, const uint32_t*> const binary = Shader::shaderBianry<T>;
    std::shared_ptr<Vulkan> vulkan = self();
    std::shared_ptr<ShaderModule> shaderModule =
        ShaderModule::create(vulkan, std::get<1>(binary), std::get<0>(binary), typeid(T).name());
    return std::make_shared<T>(vulkan, shaderModule, std::forward<Args>(args)...);
  }

};

}
