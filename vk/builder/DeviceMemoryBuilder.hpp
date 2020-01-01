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

#include "../../util/Shared.hpp"

namespace vk {

class Vulkan;
class DeviceMemory;

class DeviceMemoryBuilder final {
private:
  std::shared_ptr<Vulkan> vulkan_;
  VkMemoryAllocateInfo vkMemoryAllocateInfo_{};
  VkDeviceSize allocationSize_;
  VkMemoryRequirements requirements_{};
  VkMemoryPropertyFlags propertyFlags_{};
public:
  DeviceMemoryBuilder(DeviceMemoryBuilder&&) = delete;
  DeviceMemoryBuilder(DeviceMemoryBuilder const&) = delete;
  DeviceMemoryBuilder& operator=(DeviceMemoryBuilder&&) = delete;
  DeviceMemoryBuilder& operator=(DeviceMemoryBuilder const&) = delete;
  explicit DeviceMemoryBuilder(std::shared_ptr<Vulkan> vulkan, VkDeviceSize allocationSize,
                               const VkMemoryRequirements& requirements, VkMemoryPropertyFlags propertyFlags);

  DeviceMemory build();
};

}
