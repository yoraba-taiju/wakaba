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
class Image;

class Vulkan final : public std::enable_shared_from_this<Vulkan> {
private:
  friend class VulkanBuilder;

private: /* Util */
  util::Logger &log_;

private: /* Vulkan */
  VkInstance vkInstance_{};
  VkSurfaceKHR vkSurface_{};
  GLFWwindow* window_{};
  uint32_t width_{};
  uint32_t height_{};
  VkPhysicalDevice vkPhysicalDevice_{};
  uint32_t graphicsQueueFamiliIndex_{};
  uint32_t presentQueueFamiliIndex_{};
  VkDevice vkDevice_{};
  VkQueue vkGraphicsQueue_{};
  VkQueue vkPresentQueue_{};
  VkFence fence_{};
  VkSwapchainKHR vkSwapchain_{};
  VkSurfaceFormatKHR vkSwapchainFormat_{};
  std::vector<std::shared_ptr<Image>> swapchainImages_{};
  std::vector<std::shared_ptr<FrameBuffer>> frameBuffers_{};

private:
  VkDebugReportCallbackEXT vkDebugReportCallback_{};
  PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback_{};

public:
  explicit Vulkan(util::Logger &log);
  ~Vulkan();

public:
  [[ nodiscard ]] GLFWwindow* window() {
    return this->window_;
  }

  [[ nodiscard ]] VkInstance vkInstance() {
    return this->vkInstance_;
  }

  [[ nodiscard ]] VkDevice vkDevice() {
    return this->vkDevice_;
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

  [[ nodiscard ]] std::vector<std::shared_ptr<Image>>& swapchainImages() {
    return this->swapchainImages_;
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
