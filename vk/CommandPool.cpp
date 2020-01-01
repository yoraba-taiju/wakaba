/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "CommandPool.hpp"
#include "Vulkan.hpp"
#include "CommandBuffer.hpp"

namespace vk {

CommandPool::~CommandPool() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyCommandPool(vulkan->vkDevice(), vkCommandPool_, nullptr);
  }
}

CommandBuffer CommandPool::createBuffer() {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(!vulkan) {
    throw std::runtime_error("vulkan is already deleted.");
  }

  VkCommandBuffer vkCommandBuffer;
  {
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
        .sType =  VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = vkCommandPool(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    if (vkAllocateCommandBuffers(vulkan->vkDevice(), &commandBufferAllocateInfo, &vkCommandBuffer) != VK_SUCCESS) {
      vulkan->log().fatal("[Vulkan] Failed to create a Command Buffer.");
    }
  }
  return CommandBuffer(vulkan_, shared_from_this(), vkCommandBuffer);
}

}
