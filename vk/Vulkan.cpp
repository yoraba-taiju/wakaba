/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include <memory>
#include <vector>

#include "../util/File.hpp"

#include "Vulkan.hpp"
#include "RenderPass.hpp"
#include "FrameBuffer.hpp"
#include "ShaderModule.hpp"
#include "builder/GraphicsPipelineBuilder.hpp"
#include "CommandBuffer.hpp"
#include "CommandPool.hpp"

namespace vk {

Vulkan::Vulkan(util::Logger &log)
: log_(log)
{
}

Vulkan::~Vulkan() {
}

void Vulkan::destroy() {
  frameBuffers_.clear();
  swapchainImages_.clear();
  vkDestroySwapchainKHR(vkDevice_, vkSwapchain_, nullptr);

  vkDestroyFence(vkDevice_, fence_, nullptr);

  vkDestroyDevice(vkDevice_, nullptr);
  vkDestroySurfaceKHR(this->vkInstance_, this->vkSurface_, nullptr);
  this->vkDestroyDebugReportCallback_(this->vkInstance_, this->vkDebugReportCallback_, nullptr);

  vkDestroyInstance(this->vkInstance_, nullptr);
  glfwDestroyWindow(this->window_);
}

std::shared_ptr<CommandBuffer> Vulkan::createCommandBuffer() {
  VkCommandPool vkCommandPool;
  {
    VkCommandPoolCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = graphicsQueueFamiliIndex_,
    };

    if (vkCreateCommandPool(vkDevice_, &info, nullptr, &vkCommandPool) != VK_SUCCESS) {
      log().fatal("[Vulkan] Failed to create a command pool.");
    }
  }
  std::shared_ptr<CommandPool> commandPool = util::make_shared<CommandPool>(self(), vkCommandPool);

  VkCommandBuffer vkCommandBuffer;
  {
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
        .sType =  VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = commandPool->vkCommandPool(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    if (vkAllocateCommandBuffers(vkDevice(), &commandBufferAllocateInfo, &vkCommandBuffer) != VK_SUCCESS) {
      log().fatal("[Vulkan] Failed to create a Command Buffer.");
    }
  }
  std::shared_ptr<CommandBuffer> commandBuffer = util::make_shared<CommandBuffer>(self(), commandPool, vkCommandBuffer);
  return std::move(commandBuffer);
}

}