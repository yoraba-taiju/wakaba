//
// Created by kaede on 2019/12/30.
//

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
public:
  Image(std::shared_ptr<Vulkan> const& vulkan, VkImage vkImage, VkImageView vkImageView)
  :vulkan_(vulkan)
  ,vkImage_(vkImage)
  ,vkImageView_(vkImageView){
  }
  virtual ~Image() noexcept;
};

}


