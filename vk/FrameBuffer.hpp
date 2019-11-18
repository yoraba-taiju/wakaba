//
// Created by kaede on 2019/11/13.
//

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
class CommandBuffer;

class FrameBuffer final{
private:
  std::weak_ptr<Vulkan> vulkan_;
  VkFramebuffer vkObj_;
  std::shared_ptr<RenderPass> rendrPass_;
  std::shared_ptr<CommandBuffer> commandBuffer_;
public:
  FrameBuffer() = delete;
  ~FrameBuffer();

  explicit FrameBuffer(std::weak_ptr<Vulkan> vulkan, VkFramebuffer vkObj, std::shared_ptr<RenderPass> rendrPass, std::shared_ptr<CommandBuffer> commandBuffer)
  : vulkan_(std::move(vulkan))
  , vkObj_(vkObj)
  , rendrPass_(std::move(rendrPass))
  , commandBuffer_(std::move(commandBuffer)) {
  }

  inline VkFramebuffer vkObj() {
    return this->vkObj_;
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


