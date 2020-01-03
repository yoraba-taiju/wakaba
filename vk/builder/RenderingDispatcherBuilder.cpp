/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "RenderingDispatcherBuilder.hpp"
#include "../Vulkan.hpp"
#include "../RenderingDispatcher.hpp"
#include "../Swapchain.hpp"

namespace vk {

RenderingDispatcherBuilder::RenderingDispatcherBuilder(std::shared_ptr<Device> device, std::shared_ptr<Swapchain> swapchain)
:device_(std::move(device))
,swapchain_(std::move(swapchain))
{

}

RenderingDispatcher RenderingDispatcherBuilder::build() {
  RenderingDispatcher dispatcher(device_, swapchain_);
  VkSemaphoreCreateInfo semaphoreInfo = {
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
  };
  VkFenceCreateInfo fenceInfo = {
      .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
      .pNext = nullptr,
      .flags = VK_FENCE_CREATE_SIGNALED_BIT
  };

  for (size_t i = 0; i < RenderingDispatcher::NumFrames; i++) {
    if (vkCreateSemaphore(device_->vkDevice(), &semaphoreInfo, nullptr, &dispatcher.imageAvailableSemaphores_[i]) != VK_SUCCESS){
      throw std::runtime_error(fmt::format("failed to create imageAvailableSemaphores_[{}] for a frame!", i));
    }
    if (vkCreateSemaphore(device_->vkDevice(), &semaphoreInfo, nullptr, &dispatcher.renderFinishedSemaphores_[i]) != VK_SUCCESS){
      throw std::runtime_error(fmt::format("failed to create renderFinishedSemaphores_[{}] for a frame!", i));
    }
    if (vkCreateFence(device_->vkDevice(), &fenceInfo, nullptr, &dispatcher.fences_[i]) != VK_SUCCESS) {
      throw std::runtime_error(fmt::format("failed to create fences_[{}] for a frame!", i));
    }
  }
  dispatcher.swapchainFences_.resize(swapchain_->images().size());
  dispatcher.usedCommands_.resize(swapchain_->images().size());
  return std::move(dispatcher);
}

}
