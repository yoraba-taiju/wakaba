/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "Vulkan.hpp"
#include "buffer/Buffer.hpp"

namespace vk {

CommandBuffer::~CommandBuffer() noexcept{
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkFreeCommandBuffers(vulkan->vkDevice(), this->commandPool_->vkCommandPool(), 1, &this->vkCommandBuffer_);
  }
}

void CommandBuffer::copyBufferSync(Buffer &dst, VkDeviceSize dstOffset, Buffer &src, VkDeviceSize srcOffset, VkDeviceSize size) {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(!vulkan) {
    throw std::runtime_error("vulkan is already deleted.");
  }

  VkCommandBufferBeginInfo beginInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .pNext = nullptr,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
      .pInheritanceInfo = nullptr,
  };
  if(vkBeginCommandBuffer(this->vkCommandBuffer_, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("Failed to begin command buffer!");
  }

  {
    VkBufferCopy copyRegion = {
        .srcOffset = srcOffset,
        .dstOffset = dstOffset,
        .size = size,
    };
    vkCmdCopyBuffer(this->vkCommandBuffer_, src.vkBuffer(), dst.vkBuffer(), 1, &copyRegion);
  }
  if(vkEndCommandBuffer(this->vkCommandBuffer_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to end command buffer!");
  }

  VkSubmitInfo submitInfo = {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = &this->vkCommandBuffer_,
  };
  vkQueueSubmit(vulkan->vkGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(vulkan->vkGraphicsQueue());
}

}