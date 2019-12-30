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

#include "../util/Shared.hpp"

namespace vk {

class Vulkan;

class PipelineLayout final {
private:
  std::weak_ptr<Vulkan> vulkan_;
  VkPipelineLayout vkPipelineLayout_;
private:
  PipelineLayout(std::weak_ptr<Vulkan> vulkan, VkPipelineLayout pipelineLayout)
  : vulkan_(std::move(vulkan))
  , vkPipelineLayout_(pipelineLayout) {
  }
public:
  [[ nodiscard ]] VkPipelineLayout vkPipelineLayout() { return this->vkPipelineLayout_; }
  ~PipelineLayout() noexcept;
public:
  ENABLE_SHARED_HELPER
};

}



