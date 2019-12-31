/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vk {

class Vulkan;

class Image {
private:
  std::weak_ptr<Vulkan> vulkan_;
  VkImage vkImage_;
  VkImageView vkImageView_;

protected:
  explicit Image(std::shared_ptr<Vulkan> const& vulkan, VkImage vkImage, VkImageView vkImageView)
  :vulkan_(vulkan)
  ,vkImage_(vkImage)
  ,vkImageView_(vkImageView){
  }

public:
  Image() = delete;
  virtual ~Image() noexcept = default;

public:
  [[ nodiscard ]] std::shared_ptr<Vulkan> vulkan() {
    return this->vulkan_.lock();
  }
  [[ nodiscard ]] VkImage vkImage() {
    return this->vkImage_;
  }
  [[ nodiscard ]] VkImageView vkImageView() {
    return this->vkImageView_;
  }
};

}


