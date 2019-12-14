//
// Created by kaede on 2019/12/01.
//

#pragma once

#include <memory>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../util/Shared.hpp"

namespace vk{
class Vulkan;

class ShaderModule final {
private:
  friend class Vulkan;
  std::weak_ptr<Vulkan> vulkan_;
  VkShaderModule obj_;
private:
  ShaderModule(std::weak_ptr<Vulkan> vulkan, VkShaderModule shaderModule)
  : vulkan_(std::move(vulkan))
  , obj_(shaderModule) {
  }
public:
  ~ShaderModule() noexcept;

private:
  ENABLE_SHARED_HELPER
};

}
