//
// Created by kaede on 2019/12/01.
//

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
  VkPipelineLayout obj_;
private:
  PipelineLayout(std::weak_ptr<Vulkan> vulkan, VkPipelineLayout pipelineLayout)
  : vulkan_(std::move(vulkan))
  , obj_(pipelineLayout) {
  }
public:
  [[ nodiscard ]] VkPipelineLayout obj() { return this->obj_; };
  ~PipelineLayout() noexcept;
public:
  ENABLE_SHARED_HELPER
};

}



