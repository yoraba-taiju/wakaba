/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "RenderingDispatcherBuilder.hpp"
#include "../Vulkan.hpp"
#include "../RenderingDispatcher.hpp"

namespace vk {

RenderingDispatcherBuilder::RenderingDispatcherBuilder(std::shared_ptr<Vulkan> vulkan)
:vulkan_(std::move(vulkan))
{

}

RenderingDispatcher RenderingDispatcherBuilder::build() {
  RenderingDispatcher dispatcher(vulkan_);
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
    if (vkCreateSemaphore(vulkan_->vkDevice(), &semaphoreInfo, nullptr, &dispatcher.imageAvailableSemaphores_[i]) != VK_SUCCESS){
      vulkan_->log().fatal("failed to create imageAvailableSemaphores_[{}] for a frame!", i);
    }
    if (vkCreateSemaphore(vulkan_->vkDevice(), &semaphoreInfo, nullptr, &dispatcher.renderFinishedSemaphores_[i]) != VK_SUCCESS){
      vulkan_->log().fatal("failed to create renderFinishedSemaphores_[{}] for a frame!", i);
    }
    if (vkCreateFence(vulkan_->vkDevice(), &fenceInfo, nullptr, &dispatcher.fences_[i]) != VK_SUCCESS) {
      vulkan_->log().fatal("failed to create fences_[{}] for a frame!", i);
    }
  }
  dispatcher.swapchainFences_.resize(vulkan_->swapchainImages().size());
  return std::move(dispatcher);
}

}
