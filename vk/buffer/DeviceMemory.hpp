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

class Device;

class DeviceMemory {
private:
  std::shared_ptr<Device> device_;
  VkDeviceMemory vkDeviceMemory_;
  VkDeviceSize size_;
public:
  DeviceMemory() = delete;
  DeviceMemory(DeviceMemory const&) = delete;
  DeviceMemory(DeviceMemory&&) = delete;
  DeviceMemory& operator=(DeviceMemory const&) = delete;
  DeviceMemory& operator=(DeviceMemory&&) = delete;

  DeviceMemory(std::shared_ptr<Device> device, VkDeviceMemory vkDeviceMemory, VkDeviceSize size);
  ~DeviceMemory() noexcept;

  [[ nodiscard ]] VkDeviceMemory vkDeviceMemory() {
    return vkDeviceMemory_;
  }

  [[ nodiscard ]] VkDeviceSize size() const {
    return size_;
  }

  void sendDirect(VkDeviceSize offset, void const* src, size_t size);
};

}


