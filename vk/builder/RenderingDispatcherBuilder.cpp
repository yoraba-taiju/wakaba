/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "RenderingDispatcherBuilder.hpp"
#include "../Vulkan.hpp"
#include "../helper/RenderingDispatcher.hpp"
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

  size_t frameIdx = 0;
  for (auto& sync : dispatcher.syncInfo_) {
    if (vkCreateSemaphore(device_->vkDevice(), &semaphoreInfo, nullptr, &sync.imageAvailableSemaphore_) != VK_SUCCESS){
      throw std::runtime_error(fmt::format("failed to create imageAvailableSemaphores[{}] for a frame!", frameIdx));
    }
    if (vkCreateSemaphore(device_->vkDevice(), &semaphoreInfo, nullptr, &sync.renderFinishedSemaphore_) != VK_SUCCESS){
      throw std::runtime_error(fmt::format("failed to create renderFinishedSemaphores_[{}] for a frame!", frameIdx));
    }
    if (vkCreateFence(device_->vkDevice(), &fenceInfo, nullptr, &sync.fence_) != VK_SUCCESS) {
      throw std::runtime_error(fmt::format("failed to create fences_[{}] for a frame!", frameIdx));
    }
    sync.commandPool_ = device_->createCommandPool();
    frameIdx++;
  }
  return std::move(dispatcher);
}

}
