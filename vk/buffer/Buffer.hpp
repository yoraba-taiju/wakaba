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
  std::shared_ptr<DeviceMemory> deviceMemory_{};
  VkDeviceSize offset_{};
  VkDeviceSize size_;

public:
  Buffer() = delete;
  Buffer(Buffer const&) = delete;
  Buffer(Buffer&&) = default;
  Buffer& operator=(Buffer const&) = delete;
  Buffer& operator=(Buffer&&) = default;

  Buffer(std::shared_ptr<Vulkan> const& vulkan, VkBuffer vkBuffer, std::shared_ptr<DeviceMemory> deviceMemory, VkDeviceSize offset, VkDeviceSize size);
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

  void send(VkDeviceSize offset, void const* src, size_t size);
};

}
