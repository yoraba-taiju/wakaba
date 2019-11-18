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
  std::weak_ptr<Vulkan> vulkan_;
  VkRenderPass vkObj_;
public:
  RenderPass() = delete;
  RenderPass(std::weak_ptr<Vulkan> vulkan, VkRenderPass renderPass)
  : vulkan_(std::move(vulkan))
  , vkObj_(std::move(renderPass)) {
  }
  ~RenderPass();
  inline VkRenderPass vkObj() {
    return this->vkObj_;
  }

public:
  ENABLE_SHARED_HELPER
};

}
