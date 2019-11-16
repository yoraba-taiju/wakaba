//
// Created by kaede on 2019/11/13.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../util/Shared.hpp"
#include "CommandBuffer.hpp"

namespace vk {

class Vulkan;
class VulkanBuilder;
class RenderPass;
class CommandBuffer;

class FrameBuffer {
private:
  friend class Vulkan;
  friend class VulkanBuilder;

  VkFramebuffer obj_;
  std::shared_ptr<RenderPass> rendrPass_;
  std::shared_ptr<CommandBuffer> commandBuffer_;
public:
  FrameBuffer() = delete;

  explicit FrameBuffer(std::shared_ptr<RenderPass> rendrPass, std::shared_ptr<CommandBuffer> commandBuffer)
  : obj_(nullptr)
  , rendrPass_(std::move(rendrPass))
  , commandBuffer_(std::move(commandBuffer)) {
  }

  inline std::shared_ptr<RenderPass> renderPass() {
    return this->rendrPass_;
  }

  inline std::shared_ptr<CommandBuffer> commandBuffer() {
    return this->commandBuffer_;
  }

public:
  ENABLE_SHARED_HELPER
};

}


