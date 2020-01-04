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
#include <utility>
#include <functional>

namespace vk {

class Device;
class VulkanBuilder;
class CommandPool;
class Buffer;
class Framebuffer;
class GraphicsPipeline;

class CommandBuffer {
protected:
  std::shared_ptr<Device> device_;
  std::shared_ptr<CommandPool> commandPool_;
  VkCommandBuffer vkCommandBuffer_;
public:
  CommandBuffer() = delete;
  CommandBuffer(CommandBuffer const&) = delete;
  CommandBuffer(CommandBuffer&&) = default;
  CommandBuffer& operator=(CommandBuffer const&) = delete;
  CommandBuffer& operator=(CommandBuffer&&) = default;

protected:
  explicit CommandBuffer(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, VkCommandBuffer vkCommandBuffer)
  :device_(std::move(device))
  ,commandPool_(std::move(commandPool))
  ,vkCommandBuffer_(vkCommandBuffer)
  {
  }
  ~CommandBuffer() noexcept;

public:
  void bindPipeline(GraphicsPipeline& graphicsPipeilne);
  void bindVertexBuffer(uint32_t bindingPoint, Buffer& buffer);
  void draw(uint32_t vertices, uint32_t instances);

public:
  [[ nodiscard ]] VkCommandBuffer vkCommandBuffer() { return this->vkCommandBuffer_;}
  [[ nodiscard ]] std::shared_ptr<CommandPool> commandPool() { return this->commandPool_;}
};

}
