//
// Created by kaede on 2019/12/14.
//

#pragma once
#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vk {

class Vulkan;

class Pipeline {
private:
  std::weak_ptr<Vulkan> vulkan_;
  VkPipeline obj_;
protected:
  Pipeline(std::weak_ptr<Vulkan> vulkan, VkPipeline pipeline)
  : vulkan_(std::move(vulkan))
  , obj_(pipeline) {
  }
  ~Pipeline() noexcept;
};

}
