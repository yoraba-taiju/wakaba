//
// Created by kaede on 2019/11/13.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <utility>

#include "../util/Shared.hpp"

namespace vk {

class Vulkan;
class VulkanBuilder;

class CommandBuffer final {
private:
  std::weak_ptr<Vulkan> vulkan_;
  VkCommandBuffer vkObj_;
public:
  CommandBuffer() = delete;
  CommandBuffer(std::weak_ptr<Vulkan> vulkan, VkCommandBuffer vkObj)
  : vulkan_(std::move(vulkan))
  , vkObj_(vkObj)
  {
  }
  ~CommandBuffer();
public:
  ENABLE_SHARED_HELPER
};

}
