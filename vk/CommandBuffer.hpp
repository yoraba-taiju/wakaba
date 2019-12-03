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
class CommandPool;

class CommandBuffer final {
private:
  std::weak_ptr<Vulkan> vulkan_;
  std::shared_ptr<CommandPool> commandPool_;
  VkCommandBuffer vkObj_;
public:
  CommandBuffer() = delete;
  CommandBuffer(std::weak_ptr<Vulkan> vulkan, std::shared_ptr<CommandPool> commandPool, VkCommandBuffer vkObj)
  : vulkan_(std::move(vulkan))
  , commandPool_(std::move(commandPool))
  , vkObj_(vkObj)
  {
  }
  ~CommandBuffer() noexcept;
public:
  ENABLE_SHARED_HELPER
};

}
