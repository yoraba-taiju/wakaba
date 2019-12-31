/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include "Image.hpp"

namespace vk {

class SwapchainImage : public Image {
public:
  SwapchainImage() = delete;
  SwapchainImage(std::shared_ptr<Vulkan> const& vulkan, VkImage vkImage, VkImageView vkImageView);
  ~SwapchainImage() noexcept override;
};

}
