/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

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
  VkRenderPass vkRenderPass_;
public:
  RenderPass() = delete;
  RenderPass(std::weak_ptr<Vulkan> vulkan, VkRenderPass renderPass)
  : vulkan_(std::move(vulkan))
  , vkRenderPass_(renderPass) {
  }
  ~RenderPass() noexcept;
  [[ nodiscard ]] VkRenderPass vkRenderPass() {
    return this->vkRenderPass_;
  }

public:
  ENABLE_SHARED_HELPER
};

}
