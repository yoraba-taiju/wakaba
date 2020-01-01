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

namespace vk {

class Vulkan;
class VulkanBuilder;
class CommandPool;
class Buffer;

class CommandBuffer final {
private:
  std::weak_ptr<Vulkan> vulkan_;
  std::shared_ptr<CommandPool> commandPool_;
  VkCommandBuffer vkCommandBuffer_;
public:
  CommandBuffer() = delete;
  CommandBuffer(CommandBuffer const&) = delete;
  CommandBuffer(CommandBuffer&&) = default;
  CommandBuffer& operator=(CommandBuffer const&) = delete;
  CommandBuffer& operator=(CommandBuffer&&) = default;
  explicit CommandBuffer(std::weak_ptr<Vulkan> vulkan, std::shared_ptr<CommandPool> commandPool, VkCommandBuffer vkCommandBuffer)
  : vulkan_(std::move(vulkan))
  , commandPool_(std::move(commandPool))
  , vkCommandBuffer_(vkCommandBuffer)
  {
  }
  ~CommandBuffer() noexcept;

public:
  void copyBufferSync(Buffer& dst, VkDeviceSize dstOffset, Buffer& src, VkDeviceSize srcOffset, VkDeviceSize size);

public:
  [[ nodiscard ]] VkCommandBuffer vkCommandBuffer() { return this->vkCommandBuffer_;}
  [[ nodiscard ]] std::shared_ptr<CommandPool> commandPool() { return this->commandPool_;}
};

}
