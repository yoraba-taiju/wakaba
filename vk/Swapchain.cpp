/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Swapchain.hpp"
#include "Device.hpp"

namespace vk {

Swapchain::Swapchain(std::shared_ptr<Device> device, VkSwapchainKHR vkSwapchain, VkSurfaceFormatKHR vkSwapchainFormat, std::vector<std::shared_ptr<SwapchainImage>> images)
:device_(std::move(device))
,vkSwapchain_(vkSwapchain)
,vkSwapchainFormat_(vkSwapchainFormat)
,images_(std::move(images))
{

}

Swapchain::~Swapchain() noexcept {
  this->images_.clear();
  device_->destroySwapchain(*this);
}

std::shared_ptr<Swapchain> Swapchain::create(std::shared_ptr<Device> device, VkSwapchainKHR vkSwapchain, VkSurfaceFormatKHR vkSwapchainFormat) {
  std::vector<std::shared_ptr<SwapchainImage>> images;

  return std::make_shared<Swapchain>(std::move(device), vkSwapchain, vkSwapchainFormat, std::move(images));
}

}