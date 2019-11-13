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

class RenderPass {
private:
  friend class Vulkan;

  friend class VulkanBuilder;

  VkRenderPass obj_;
public:
  ENABLE_SHARED_HELPER
};

}
