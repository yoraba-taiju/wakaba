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

class PipelineLayout final {
private:
  std::shared_ptr<Device> device_;
  VkPipelineLayout vkPipelineLayout_;
public:
  PipelineLayout() = delete;
  PipelineLayout(PipelineLayout const&) = delete;
  PipelineLayout(PipelineLayout&&) = default;
  PipelineLayout& operator=(PipelineLayout const&) = delete;
  PipelineLayout& operator=(PipelineLayout&&) = default;

  explicit PipelineLayout(std::shared_ptr<Device> device, VkPipelineLayout pipelineLayout);
  ~PipelineLayout() noexcept;

  [[ nodiscard ]] VkPipelineLayout vkPipelineLayout() { return this->vkPipelineLayout_; }
};

}



