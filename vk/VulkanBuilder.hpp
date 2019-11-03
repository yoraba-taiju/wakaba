//
// Created by kaede on 2019/10/30.
//

#pragma once

#include <memory>
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../util/Logger.hpp"

namespace vk {
class Vulkan;

class VulkanBuilder final {
private:
  util::Logger& log_;
  std::shared_ptr<Vulkan> vulkan_;
  std::string appName_;
public:
  VulkanBuilder(util::Logger& log, std::string appName);
  std::shared_ptr<Vulkan> create();
};

}
