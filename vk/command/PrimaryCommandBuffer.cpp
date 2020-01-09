/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */
#include <stdexcept>
#include "PrimaryCommandBuffer.hpp"
#include "SecondaryCommandBuffer.hpp"
#include "../Framebuffer.hpp"
#include "../RenderPass.hpp"
#include "../Device.hpp"
#include "../buffer/Buffer.hpp"
namespace vk {

PrimaryCommandBuffer& PrimaryCommandBuffer::record(SecondaryCommandBuffer& secondaryBuffer) {
  VkCommandBuffer buff = secondaryBuffer.vkCommandBuffer();
  vkCmdExecuteCommands(vkCommandBuffer(), 1, &buff);
  return *this;
}

PrimaryCommandBuffer& PrimaryCommandBuffer::recordRenderPass(Framebuffer& framebuffer, std::function<void()> const& f) {
  this->record([&]() -> void {
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

    vkCmdBeginRenderPass(this->vkCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    try {
      f();
    } catch(...) {
      throw;
    }
    vkCmdEndRenderPass(this->vkCommandBuffer());
  });
  return *this;
}

PrimaryCommandBuffer& PrimaryCommandBuffer::recordRenderPass(Framebuffer& framebuffer, std::vector<SecondaryCommandBuffer>& cmds) {
  this->record([&]() -> void {
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

    std::vector<VkCommandBuffer> cmdsToSend;
    for(auto& cmd : cmds) {
      cmdsToSend.emplace_back(cmd.vkCommandBuffer());
    }
    vkCmdBeginRenderPass(this->vkCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
    vkCmdExecuteCommands(this->vkCommandBuffer(), cmdsToSend.size(), cmdsToSend.data());
    vkCmdEndRenderPass(this->vkCommandBuffer());
  });
  return *this;
}

PrimaryCommandBuffer& PrimaryCommandBuffer::recordOneshot(std::function<void()> const& f) {
  VkCommandBufferBeginInfo beginInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .pNext = nullptr,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
      .pInheritanceInfo = nullptr,
  };
  if(vkBeginCommandBuffer(this->vkCommandBuffer(), &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("Failed to begin command buffer!");
  }

  try {
    f();
  } catch(...) {
    throw;
  }

  if(vkEndCommandBuffer(this->vkCommandBuffer()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to end command buffer!");
  }
  return *this;
}

PrimaryCommandBuffer& PrimaryCommandBuffer::record(std::function<void()> const& f) {

  VkCommandBufferBeginInfo beginInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .pNext = nullptr,
      .flags = 0,
      .pInheritanceInfo = nullptr,
  };
  if(vkBeginCommandBuffer(this->vkCommandBuffer(), &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("Failed to begin command buffer!");
  }

  try {
    f();
  } catch(...) {
    throw;
  }

  if(vkEndCommandBuffer(this->vkCommandBuffer()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to end command buffer!");
  }
  return *this;
}

PrimaryCommandBuffer& PrimaryCommandBuffer::copyBufferSync(Buffer &dst, VkDeviceSize dstOffset, Buffer &src, VkDeviceSize srcOffset, VkDeviceSize size) {
  this->recordOneshot([&]() -> void {
    VkBufferCopy copyRegion = {
        .srcOffset = srcOffset,
        .dstOffset = dstOffset,
        .size = size,
    };
    vkCmdCopyBuffer(this->vkCommandBuffer(), src.vkBuffer(), dst.vkBuffer(), 1, &copyRegion);
  });
  return this->executeSync();
}
PrimaryCommandBuffer& PrimaryCommandBuffer::executeSync() {
  VkCommandBuffer buffers[] = {this->vkCommandBuffer()};
  VkSubmitInfo submitInfo = {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = buffers,
  };
  vkQueueSubmit(device_->vkGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(device_->vkGraphicsQueue());
  return *this;
}

}
