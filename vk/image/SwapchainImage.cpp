/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "SwapchainImage.hpp"
#include "../Device.hpp"

namespace vk {
SwapchainImage::SwapchainImage(std::shared_ptr<Device> device, VkImage vkImage, VkImageView vkImageView, uint32_t width, uint32_t height)
:Image(std::move(device), vkImage, vkImageView, width, height)
{

}

SwapchainImage::~SwapchainImage() noexcept {
  vkDestroyImageView(device()->vkDevice(), vkImageView(), nullptr);
  //vkDestroyImage(device()->vkDevice(), vkImage(), nullptr);
}

}