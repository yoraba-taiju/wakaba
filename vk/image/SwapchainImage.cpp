/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "SwapchainImage.hpp"
#include "../Vulkan.hpp"

namespace vk {
SwapchainImage::SwapchainImage(std::shared_ptr<Vulkan> const &vulkan, VkImage vkImage, VkImageView vkImageView)
:Image(vulkan, vkImage, vkImageView)
{

}

SwapchainImage::~SwapchainImage() noexcept {
  std::shared_ptr<Vulkan> vulkan = this->vulkan();
  if(vulkan) {
    vkDestroyImageView(vulkan->vkDevice(), vkImageView(), nullptr);
    //vkDestroyImage(vulkan->vkDevice(), vkImage(), nullptr);
  }
}

}