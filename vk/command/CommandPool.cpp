/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "CommandPool.hpp"
#include "../Vulkan.hpp"
#include "PrimaryCommandBuffer.hpp"
#include "SecondaryCommandBuffer.hpp"

namespace vk {

CommandPool::~CommandPool() noexcept {
  device_->destroyCommandPool(*this);
}

PrimaryCommandBuffer CommandPool::createPrimaryBuffer() {
  VkCommandBuffer vkCommandBuffer;
  {
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
        .sType =  VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = vkCommandPool_,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    if (vkAllocateCommandBuffers(device_->vkDevice(), &commandBufferAllocateInfo, &vkCommandBuffer) != VK_SUCCESS) {
      throw std::runtime_error(fmt::format("Failed to create a Command Buffer."));
    }
  }
  return PrimaryCommandBuffer(device_, shared_from_this(), vkCommandBuffer);
}

SecondaryCommandBuffer CommandPool::createSecondaryBuffer() {
  VkCommandBuffer vkCommandBuffer;
  {
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
        .sType =  VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = vkCommandPool_,
        .level = VK_COMMAND_BUFFER_LEVEL_SECONDARY,
        .commandBufferCount = 1
    };

    if (vkAllocateCommandBuffers(device_->vkDevice(), &commandBufferAllocateInfo, &vkCommandBuffer) != VK_SUCCESS) {
      throw std::runtime_error(fmt::format("Failed to create a Command Buffer."));
    }
  }
  return SecondaryCommandBuffer(device_, shared_from_this(), vkCommandBuffer);
}
}
