/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>
#include <optional>

namespace vk {

class Vulkan;
class DeviceMemory;

class Buffer {
private:
  std::weak_ptr<Vulkan> vulkan_;
  VkBuffer vkBuffer_;
  VkDeviceSize size_;
  std::shared_ptr<DeviceMemory> deviceMemory_{};

public:
  Buffer(std::shared_ptr<Vulkan> const& vulkan, VkBuffer vkBuffer, VkDeviceSize size);
  ~Buffer() noexcept;

public:
  [[ nodiscard ]] std::shared_ptr<Vulkan> vulkan() {
    return this->vulkan_.lock();
  }

  [[ nodiscard ]] VkBuffer vkBuffer() {
    return vkBuffer_;
  }

  [[ nodiscard ]] VkDeviceSize size() const {
    return size_;
  }

public:
  void bind(std::shared_ptr<DeviceMemory> const& deviceMemory, VkDeviceSize offset = 0);
};

}
