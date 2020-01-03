/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "../Vulkan.hpp"
#include "../buffer/Buffer.hpp"
#include "../Framebuffer.hpp"
#include "../RenderPass.hpp"
#include "../GraphicsPipeline.hpp"

namespace vk {

CommandBuffer::~CommandBuffer() noexcept{
  if(device_) {
    device_->destroyCommandBuffer(*this);
  }
}

void CommandBuffer::copyBufferSync(Buffer &dst, VkDeviceSize dstOffset, Buffer &src, VkDeviceSize srcOffset, VkDeviceSize size) {
  this->recordOneshot([&](std::shared_ptr<Device> const&) -> void {
    VkBufferCopy copyRegion = {
        .srcOffset = srcOffset,
        .dstOffset = dstOffset,
        .size = size,
    };
    vkCmdCopyBuffer(this->vkCommandBuffer_, src.vkBuffer(), dst.vkBuffer(), 1, &copyRegion);
  });
  this->executeSync();
}

void CommandBuffer::recordOneshot(std::function<void(std::shared_ptr<Device> const&)> const& f) {
  VkCommandBufferBeginInfo beginInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .pNext = nullptr,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
      .pInheritanceInfo = nullptr,
  };
  if(vkBeginCommandBuffer(this->vkCommandBuffer_, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("Failed to begin command buffer!");
  }

  try {
    f(device_);
  } catch(...) {
    throw;
  }

  if(vkEndCommandBuffer(this->vkCommandBuffer_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to end command buffer!");
  }
}

void CommandBuffer::recordRenderPass(Framebuffer& framebuffer, std::function<void(std::shared_ptr<Device> const&)> const& f) {
  this->record([&](std::shared_ptr<Device> const&) -> void {

    VkRenderPassBeginInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = nullptr,
        .renderPass = framebuffer.renderPass()->vkRenderPass(),
        .framebuffer = framebuffer.vkFramebuffer(),
        .renderArea = {
            .offset = {0, 0},
            .extent = {framebuffer.width(), framebuffer.height()},
        },
        .clearValueCount = static_cast<uint32_t>(framebuffer.clears().size()),
        .pClearValues = framebuffer.clears().data(),
    };

    vkCmdBeginRenderPass(vkCommandBuffer_, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    try {
      f(device_);
    } catch(...) {
      throw;
    }
    vkCmdEndRenderPass(vkCommandBuffer_);

  });
}

void CommandBuffer::bindPipeline(GraphicsPipeline& graphicsPipeilne) {
  vkCmdBindPipeline(vkCommandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeilne.vkPipeline());
}

void CommandBuffer::bindVertexBuffer(uint32_t const bindingPoint, Buffer& buffer) {
  VkBuffer vertexBuffers[] = {buffer.vkBuffer()};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(vkCommandBuffer_, bindingPoint, 1, vertexBuffers, offsets);
}

void CommandBuffer::draw(uint32_t const vertices, uint32_t const instances) {
  vkCmdDraw(vkCommandBuffer_, vertices, 1, 0, 0);
}

void CommandBuffer::record(std::function<void(std::shared_ptr<Device> const&)> const& f) {

  VkCommandBufferBeginInfo beginInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .pNext = nullptr,
      .flags = 0,
      .pInheritanceInfo = nullptr,
  };
  if(vkBeginCommandBuffer(this->vkCommandBuffer_, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("Failed to begin command buffer!");
  }

  try {
    f(device_);
  } catch(...) {
    throw;
  }

  if(vkEndCommandBuffer(this->vkCommandBuffer_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to end command buffer!");
  }
}

void CommandBuffer::executeSync() {
  VkSubmitInfo submitInfo = {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = &this->vkCommandBuffer_,
  };
  vkQueueSubmit(device_->vkGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(device_->vkGraphicsQueue());
}


}