//
// Created by kaede on 2019/11/18.
//

#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <utility>
#include "../util/Shared.hpp"

namespace vk {

class Vulkan;

class CommandPool {
private:
  std::weak_ptr<Vulkan> vulkan_;
  VkCommandPool obj_;
public:
  CommandPool(std::weak_ptr<Vulkan> vulkan, VkCommandPool vkObj)
  : vulkan_(std::move(vulkan))
  , obj_(vkObj)
  {

  }
  ~CommandPool() noexcept;

public:
  inline VkCommandPool vkObj() {
    return this->obj_;
  }
public:
  ENABLE_SHARED_HELPER
};

}
