//
// Created by kaede on 2019/10/29.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../util/Shared.hpp"

namespace vk {

class Vulkan {
private:
  VkInstance instance_;
private:
  explicit Vulkan();
public:
  static std::shared_ptr<Vulkan> createInstance(std::string const& appName);
  ENABLE_SHARED_HELPER
};

}
