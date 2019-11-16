//
// Created by kaede on 2019/11/13.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../util/Shared.hpp"

namespace vk {

class Vulkan;
class VulkanBuilder;

class CommandBuffer {
private:
  friend class Vulkan;
  friend class VulkanBuilder;
  VkCommandBuffer obj_;
public:
  ENABLE_SHARED_HELPER
};

}
