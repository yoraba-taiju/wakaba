/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <utility>
#include <memory>
#include "CommandBuffer.hpp"

namespace vk {

class Vulkan;
class VulkanBuilder;
class RenderPass;
class CommandPool;
class CommandBuffer;

class Framebuffer final {
private:
  std::weak_ptr<Vulkan> vulkan_;
  VkFramebuffer vkFramebuffer_;
  std::shared_ptr<RenderPass> rendrPass_;
public:
  Framebuffer() = delete;
  Framebuffer(Framebuffer const&) = delete;
  Framebuffer(Framebuffer&&) = default;
  Framebuffer& operator=(Framebuffer const&) = delete;
  Framebuffer& operator=(Framebuffer&&) = default;
  ~Framebuffer() noexcept;

  explicit Framebuffer(std::weak_ptr<Vulkan> vulkan, VkFramebuffer vkFramebuffer, std::shared_ptr<RenderPass> rendrPass)
  :vulkan_(std::move(vulkan))
  ,vkFramebuffer_(vkFramebuffer)
  ,rendrPass_(std::move(rendrPass))
  {

  }

  [[ nodiscard ]] VkFramebuffer vkFramebuffer() {
    return this->vkFramebuffer_;
  }

  [[ nodiscard ]] std::shared_ptr<RenderPass> renderPass() {
    return this->rendrPass_;
  }

};

}


