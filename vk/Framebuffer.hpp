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
#include "command/CommandBuffer.hpp"

namespace vk {

class Vulkan;
class Device;
class RenderPass;
class CommandPool;
class CommandBuffer;
class Image;

class Framebuffer final {
private:
  std::shared_ptr<Device> device_;
  VkFramebuffer vkFramebuffer_;
  std::shared_ptr<RenderPass> renderPass_;
  std::vector<std::shared_ptr<Image>> images_;
  std::vector<VkClearValue> clears_;
  uint32_t width_;
  uint32_t height_;
public:
  Framebuffer() = delete;
  Framebuffer(Framebuffer const&) = delete;
  Framebuffer(Framebuffer&&) = default;
  Framebuffer& operator=(Framebuffer const&) = delete;
  Framebuffer& operator=(Framebuffer&&) = default;
  ~Framebuffer() noexcept;

public:
  explicit Framebuffer(std::shared_ptr<Device> device, VkFramebuffer vkFramebuffer, std::shared_ptr<RenderPass> renderPass, std::vector<std::shared_ptr<Image>> images, std::vector<VkClearValue> clears, uint32_t width, uint32_t height)
  :device_(std::move(device))
  ,vkFramebuffer_(vkFramebuffer)
  ,renderPass_(std::move(renderPass))
  ,images_(std::move(images))
  ,clears_(std::move(clears))
  ,width_(width)
  ,height_(height)
  {
  }

  [[ nodiscard ]] VkFramebuffer vkFramebuffer() {
    return this->vkFramebuffer_;
  }

  [[ nodiscard ]] std::shared_ptr<RenderPass> renderPass() {
    return this->renderPass_;
  }

  [[ nodiscard ]] std::vector<std::shared_ptr<Image>> const& images() const {
    return this->images_;
  }

  [[ nodiscard ]] std::vector<VkClearValue> const& clears() const {
    return this->clears_;
  }

  [[ nodiscard ]] uint32_t width() const {
    return this->width_;
  }

  [[ nodiscard ]] uint32_t height() const {
    return this->height_;
  }

};

}


