/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "RenderingDispatcher.hpp"
#include "../Vulkan.hpp"
#include "../Swapchain.hpp"
#include "../command/CommandPool.hpp"

namespace vk {

RenderingDispatcher::RenderingDispatcher(std::shared_ptr<Device> device, std::shared_ptr<Swapchain> swapchain)
:device_(std::move(device))
,swapchain_(std::move(swapchain))
{

}

RenderingDispatcher::~RenderingDispatcher() noexcept {
  if (device_) {
    for (auto& sync : this->syncInfo_) {
      // wait for sync.
      vkWaitForFences(device_->vkDevice(), 1, &sync.fence_, VK_TRUE, UINT64_MAX);
      sync.clear();
      //
      vkDestroyFence(device_->vkDevice(), sync.fence_, nullptr);
      vkDestroySemaphore(device_->vkDevice(), sync.imageAvailableSemaphore_, nullptr);
      vkDestroySemaphore(device_->vkDevice(), sync.renderFinishedSemaphore_, nullptr);
    }
  }
}

void RenderingDispatcher::dispatch(std::function<void(std::shared_ptr<CommandPool> const&, uint32_t)> const& f) {
  FrameSyncInfo& sync = this->syncInfo_[currentFrame_];
  VkResult result = vkAcquireNextImageKHR(device_->vkDevice(), swapchain_->vkSwapchain(), UINT64_MAX, sync.imageAvailableSemaphore_, VK_NULL_HANDLE, &sync.imageIndex_);

  // VK_SUBOPTIMAL_KHR A swapchain no longer matches the surface properties exactly,
  // but can still be used to present to the surface successfully.
  // https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkResult.html
  if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  // Wait for finishing rendering.
  vkWaitForFences(device_->vkDevice(), 1, &sync.fence_, VK_TRUE, UINT64_MAX);
  sync.clear();

  VkSemaphore waitSemaphores[] = {sync.imageAvailableSemaphore_};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  VkSemaphore signalSemaphores[] = {sync.renderFinishedSemaphore_};

  f(sync.commandPool_, sync.imageIndex_);

  std::vector<VkCommandBuffer> cmds;
  for(CommandBuffer& cmd : sync.primaryBuffers_) {
    cmds.emplace_back(cmd.vkCommandBuffer());
  }

  VkSubmitInfo submitInfo = {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .pNext = nullptr,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = waitSemaphores,
      .pWaitDstStageMask = waitStages,
      .commandBufferCount = static_cast<uint32_t>(cmds.size()),
      .pCommandBuffers = cmds.data(),
      .signalSemaphoreCount = 1,
      .pSignalSemaphores = signalSemaphores,
  };

  vkResetFences(device_->vkDevice(), 1, &sync.fence_);

  if (vkQueueSubmit(device_->vkGraphicsQueue(), 1, &submitInfo, sync.fence_) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkSwapchainKHR swapChains[] = { swapchain_->vkSwapchain() };
  VkPresentInfoKHR presentInfo = {
      .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .pNext = nullptr,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = signalSemaphores,
      .swapchainCount = 1,
      .pSwapchains = swapChains,
      .pImageIndices = &sync.imageIndex_,
      .pResults = nullptr,
  };

  // https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkQueuePresentKHR.html
  result = vkQueuePresentKHR(device_->vkPresentQueue(), &presentInfo);
  if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to present image!");
  }

  currentFrame_ = (currentFrame_ + 1) % NumFrames;
}

RenderingDispatcher& RenderingDispatcher::submit(PrimaryCommandBuffer&& commandBuffer) {
  this->syncInfo_[currentFrame_].primaryBuffers_.emplace_back(std::move(commandBuffer));
  return *this;
}

RenderingDispatcher& RenderingDispatcher::submit(SecondaryCommandBuffer&& commandBuffer) {
  this->syncInfo_[currentFrame_].secondaryBuffers_.emplace_back(std::move(commandBuffer));
  return *this;
}

}