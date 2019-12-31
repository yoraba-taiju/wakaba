/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <utility>
#include "../util/Shared.hpp"
#include "CommandBuffer.hpp"

namespace vk {

class Vulkan;
class VulkanBuilder;
class RenderPass;
class CommandPool;
class CommandBuffer;

class FrameBuffer final{
private:
  std::weak_ptr<Vulkan> vulkan_;
  VkFramebuffer vkFramebuffer_;
  std::shared_ptr<RenderPass> rendrPass_;
  std::shared_ptr<CommandPool> commandPool_;
  std::shared_ptr<CommandBuffer> commandBuffer_;
public:
  FrameBuffer() = delete;
  ~FrameBuffer() noexcept;

  explicit FrameBuffer(std::weak_ptr<Vulkan> vulkan, VkFramebuffer vkFramebuffer, std::shared_ptr<RenderPass> rendrPass, std::shared_ptr<CommandPool> commandPool, std::shared_ptr<CommandBuffer> commandBuffer)
  : vulkan_(std::move(vulkan))
  , vkFramebuffer_(vkFramebuffer)
  , rendrPass_(std::move(rendrPass))
  , commandPool_(std::move(commandPool))
  , commandBuffer_(std::move(commandBuffer)) {
  }

  [[ nodiscard ]] VkFramebuffer vkFramebuffer() {
    return this->vkFramebuffer_;
  }

  [[ nodiscard ]] std::shared_ptr<RenderPass> renderPass() {
    return this->rendrPass_;
  }

  [[ nodiscard ]] std::shared_ptr<CommandBuffer> commandBuffer() {
    return this->commandBuffer_;
  }

public:
  ENABLE_SHARED_HELPER
};

}


