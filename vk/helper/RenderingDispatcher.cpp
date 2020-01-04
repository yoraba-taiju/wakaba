/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "RenderingDispatcher.hpp"
#include "../Vulkan.hpp"
#include "../Swapchain.hpp"

namespace vk {

RenderingDispatcher::RenderingDispatcher(std::shared_ptr<Device> device, std::shared_ptr<Swapchain> swapchain)
:device_(std::move(device))
,swapchain_(std::move(swapchain))
{

}

void RenderingDispatcher::dispatch(std::function<void(RenderingDispatcher&, uint32_t)> const& f) {
  vkWaitForFences(device_->vkDevice(), 1, &fences_[(currentFrame_ + NumFrames - 1) % NumFrames], VK_TRUE, UINT64_MAX);

  VkResult result = vkAcquireNextImageKHR(device_->vkDevice(), swapchain_->vkSwapchain(), UINT64_MAX, imageAvailableSemaphores_[currentFrame_], VK_NULL_HANDLE, &currentImageIndex_);

  // VK_SUBOPTIMAL_KHR A swapchain no longer matches the surface properties exactly,
  // but can still be used to present to the surface successfully.
  // https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkResult.html
  if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  if (swapchainFences_[currentImageIndex_] != VK_NULL_HANDLE) {
    vkWaitForFences(device_->vkDevice(), 1, &swapchainFences_[currentImageIndex_], VK_TRUE, UINT64_MAX);
  }
  swapchainFences_[currentImageIndex_] = fences_[currentFrame_];

  VkSemaphore waitSemaphores[] = {imageAvailableSemaphores_[currentFrame_]};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  VkSemaphore signalSemaphores[] = {renderFinishedSemaphores_[currentFrame_]};

  f(*this, currentImageIndex_);

  this->usedCommands_[currentImageIndex_] = std::move(this->commands_[currentFrame_]);
  std::vector<VkCommandBuffer> cmds;
  for(CommandBuffer& cmd : this->usedCommands_[currentImageIndex_]) {
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

  vkResetFences(device_->vkDevice(), 1, &swapchainFences_[currentImageIndex_]);

  if (vkQueueSubmit(device_->vkGraphicsQueue(), 1, &submitInfo, swapchainFences_[currentImageIndex_]) != VK_SUCCESS) {
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
      .pImageIndices = &currentImageIndex_,
      .pResults = nullptr,
  };

  // https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkQueuePresentKHR.html
  result = vkQueuePresentKHR(device_->vkPresentQueue(), &presentInfo);
  if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
    throw std::runtime_error("failed to present image!");
  }
  vkWaitForFences(device_->vkDevice(), 1, &fences_[currentFrame_], VK_TRUE, UINT64_MAX);

  currentFrame_ = (currentFrame_ + 1) % NumFrames;
}

RenderingDispatcher &RenderingDispatcher::push(PrimaryCommandBuffer&& commandBuffer) {
  this->commands_[currentFrame_].emplace_back(std::move(commandBuffer));
  return *this;
}

}