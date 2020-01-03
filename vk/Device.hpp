/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

#include "shader/Shader.hpp"

namespace vk {
class Vulkan;
class Framebuffer;
class RenderPass;
class ShaderModule;
class CommandPool;
class Queue;
class PipelineLayout;
class Swapchain;
class Pipeline;
class Buffer;
class DeviceMemory;
class CommandBuffer;

class Device final : public std::enable_shared_from_this<Device> {
private:
  std::shared_ptr<Vulkan> vulkan_;
  VkDevice vkDevice_;
  VkQueue vkGraphicsQueue_;
  VkQueue vkPresentQueue_;
public:
  Device() = delete;
  Device(Device const&) = delete;
  Device(Device&&) = default;
  Device& operator=(Device const&) = delete;
  Device& operator=(Device&&) = default;

public:
  explicit Device(std::shared_ptr<Vulkan> vulkan, VkDevice device, VkQueue vkGraphicsQueue, VkQueue VkPresentQueue);
  ~Device() noexcept;

public:
  [[ nodiscard ]] std::shared_ptr<Vulkan> vulkan() {
    return this->vulkan_;
  }
  [[ nodiscard ]] VkDevice vkDevice() {
    return this->vkDevice_;
  }
  [[ nodiscard ]] VkQueue vkGraphicsQueue() {
    return this->vkGraphicsQueue_;
  }
  [[ nodiscard ]] VkQueue vkPresentQueue() {
    return this->vkPresentQueue_;
  }
  [[ nodiscard ]] uint32_t vkGraphicsQueueFamilyIndex();
  [[ nodiscard ]] uint32_t vkPresentQueueFamilyIndex();
  [[ nodiscard ]] std::shared_ptr<Swapchain> createSwapchain();
  [[ nodiscard ]] std::shared_ptr<CommandPool> createCommandPool();
public:

  template<typename T, typename... Args>
  std::shared_ptr<T> createShader(Args &&... args) {
    static_assert(std::is_base_of<Shader, T>::value);
    std::tuple<size_t, const uint32_t*> const binary = Shader::loadBianry<T>();
    std::shared_ptr<Device> device = shared_from_this();
    ShaderModule module = ShaderModule::create(device, std::get<1>(binary), std::get<0>(binary), typeid(T).name());
    return std::make_shared<T>(device, std::move(module), std::forward<Args>(args)...);
  }

public:
  void destroyCommandPool(CommandPool& commandPool);
  void destroySwapchain(Swapchain& swapchain);
  void destroyFramebuffer(Framebuffer& framebuffer);
  void destroyRenderPass(RenderPass& renderPass);
  void destroyShaderModule(ShaderModule& shaderModule);
  void destroyPipelineLayout(PipelineLayout& pipelineLayout);
  void destroyPipeline(Pipeline& pipeline);
  void destroyBuffer(Buffer& buffer);
  void destroyDeviceMemory(DeviceMemory& deviceMemory);
  void destroyCommandBuffer(CommandBuffer& commandBuffer);
};

}
