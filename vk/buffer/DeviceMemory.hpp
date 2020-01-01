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

class DeviceMemory {
private:
  std::weak_ptr<Vulkan> vulkan_;
  VkDeviceMemory vkDeviceMemory_;
  VkDeviceSize size_;
public:
  DeviceMemory() = delete;
  DeviceMemory(DeviceMemory const&) = delete;
  DeviceMemory(DeviceMemory&&) = default;
  DeviceMemory& operator=(DeviceMemory const&) = delete;
  DeviceMemory& operator=(DeviceMemory&&) = default;

  DeviceMemory(std::shared_ptr<Vulkan> const& vulkan, VkDeviceMemory vkDeviceMemory, VkDeviceSize size);
  ~DeviceMemory() noexcept;

  [[ nodiscard ]] std::shared_ptr<Vulkan> vulkan() {
    return this->vulkan_.lock();
  }

  [[ nodiscard ]] VkDeviceMemory vkDeviceMemory() {
    return vkDeviceMemory_;
  }

  [[ nodiscard ]] VkDeviceSize size() const {
    return size_;
  }
};

}


