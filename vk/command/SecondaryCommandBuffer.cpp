/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include <stdexcept>
#include "SecondaryCommandBuffer.hpp"
#include "../Framebuffer.hpp"
#include "../RenderPass.hpp"

namespace vk {

SecondaryCommandBuffer& SecondaryCommandBuffer::recordOneshot(Framebuffer &framebuffer, std::function<void()> const& f) {
  VkCommandBufferInheritanceInfo vkCommandBufferInheritanceInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
      .pNext = nullptr,
      .renderPass = framebuffer.renderPass()->vkRenderPass(),
      .subpass = 0, //FIXME
      .framebuffer = framebuffer.vkFramebuffer(),
      .occlusionQueryEnable = VK_FALSE,
      .queryFlags = 0,
      .pipelineStatistics = 0,
  };
  VkCommandBufferBeginInfo beginInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .pNext = nullptr,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
      .pInheritanceInfo = &vkCommandBufferInheritanceInfo,
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

SecondaryCommandBuffer& SecondaryCommandBuffer::record(Framebuffer &framebuffer, std::function<void()> const& f) {
  VkCommandBufferInheritanceInfo vkCommandBufferInheritanceInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
      .pNext = nullptr,
      .renderPass = framebuffer.renderPass()->vkRenderPass(),
      .subpass = 0, //FIXME
      .framebuffer = framebuffer.vkFramebuffer(),
      .occlusionQueryEnable = VK_FALSE,
      .queryFlags = 0,
      .pipelineStatistics = 0,
  };
  VkCommandBufferBeginInfo beginInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .pNext = nullptr,
      .flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
      .pInheritanceInfo = &vkCommandBufferInheritanceInfo,
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

}
