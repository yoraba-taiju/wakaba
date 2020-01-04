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
class DeviceMemory;
class PrimaryCommandBuffer;

class Buffer {
private:
  std::shared_ptr<Device> device_;
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

  Buffer(std::shared_ptr<Device> device, VkBuffer vkBuffer, VkDeviceSize size);
  ~Buffer() noexcept;

public:

  [[ nodiscard ]] VkBuffer vkBuffer() {
    return vkBuffer_;
  }

  [[ nodiscard ]] VkDeviceSize size() const {
    return size_;
  }

  void bindTo(std::shared_ptr<DeviceMemory> devMem, VkDeviceSize offset);

  void sendDirect(VkDeviceSize offset, void const* src, size_t size);
  void sendIndirect(PrimaryCommandBuffer& cmdBuffer, VkDeviceSize offset, void const* src, size_t size);

  VkMemoryRequirements vkMemoryRequirements();
};

}
