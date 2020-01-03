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

class Device;

class Image {
private:
  std::shared_ptr<Device> device_;
  VkImage vkImage_;
  VkImageView vkImageView_;
  uint32_t const width_;
  uint32_t const height_;
protected:
  explicit Image(std::shared_ptr<Device> device, VkImage vkImage, VkImageView vkImageView, uint32_t width, uint32_t height)
  :device_(std::move(device))
  ,vkImage_(vkImage)
  ,vkImageView_(vkImageView)
  ,width_(width)
  ,height_(height)
  {
  }

public:
  Image() = delete;
  Image(Image const&) = delete;
  Image(Image&&) = delete;
  Image& operator=(Image const&) = delete;
  Image& operator=(Image&&) = delete;
  virtual ~Image() noexcept = default;

protected:

public:
  [[ nodiscard ]] std::shared_ptr<Device> device() {
    return this->device_;
  }
  [[ nodiscard ]] VkImage vkImage() {
    return this->vkImage_;
  }
  [[ nodiscard ]] VkImageView vkImageView() {
    return this->vkImageView_;
  }
  [[ nodiscard ]] uint32_t width() {
    return this->width_;
  }
  [[ nodiscard ]] uint32_t height() {
    return this->height_;
  }
};

}


