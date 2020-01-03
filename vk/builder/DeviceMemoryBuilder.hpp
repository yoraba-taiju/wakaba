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

class DeviceMemoryBuilder final {
private:
  std::shared_ptr<Device> device_;
  VkMemoryAllocateInfo vkMemoryAllocateInfo_{};
  VkMemoryRequirements requirements_{};
  VkMemoryPropertyFlags propertyFlags_{};
public:
  DeviceMemoryBuilder() = delete;
  DeviceMemoryBuilder(DeviceMemoryBuilder&&) = delete;
  DeviceMemoryBuilder(DeviceMemoryBuilder const&) = delete;
  DeviceMemoryBuilder& operator=(DeviceMemoryBuilder&&) = delete;
  DeviceMemoryBuilder& operator=(DeviceMemoryBuilder const&) = delete;
  explicit DeviceMemoryBuilder(std::shared_ptr<Device> device, VkMemoryRequirements const& requirements, VkMemoryPropertyFlags propertyFlags);

  std::shared_ptr<DeviceMemory> build();
};

}
