/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vk {

class Device;

class RenderPass {
private:
  std::shared_ptr<Device> device_;
  VkRenderPass vkRenderPass_;
public:
  RenderPass() = delete;
  RenderPass(RenderPass const&) = delete;
  RenderPass(RenderPass&&) = default;
  RenderPass& operator=(RenderPass const&) = delete;
  RenderPass& operator=(RenderPass&&) = default;

  explicit RenderPass(std::shared_ptr<Device> device, VkRenderPass renderPass);
  ~RenderPass() noexcept;

  [[ nodiscard ]] VkRenderPass vkRenderPass() {
    return this->vkRenderPass_;
  }

};

}
