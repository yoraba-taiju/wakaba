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
  explicit SwapchainImage(std::shared_ptr<Vulkan> const& vulkan, VkImage vkImage, VkImageView vkImageView, uint32_t width, uint32_t height);
  ~SwapchainImage() noexcept override;
};

}
