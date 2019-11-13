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
class RenderPass;

class FrameBuffer {
private:
  friend class Vulkan;
  friend class VulkanBuilder;
  VkFramebuffer obj_;
  std::shared_ptr<RenderPass> rendrPass_;
public:
  FrameBuffer() = delete;
  explicit FrameBuffer(std::shared_ptr<RenderPass> rendrPass)
  : obj_(nullptr)
  ,rendrPass_(std::move(rendrPass)) {
  }
public:
  ENABLE_SHARED_HELPER
};

}


