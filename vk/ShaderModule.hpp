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
  std::string name_;
  VkShaderModule obj_;
private:
  ShaderModule(std::weak_ptr<Vulkan> vulkan, std::string name, VkShaderModule shaderModule)
  : vulkan_(std::move(vulkan))
  , name_(std::move(name))
  , obj_(shaderModule) {
  }

public:
  static std::shared_ptr<ShaderModule> create(std::shared_ptr<Vulkan> const& vulkan, uint32_t const* code, size_t length, const char* className) noexcept (false);

public:
  ~ShaderModule() noexcept;
  [[ nodiscard ]] VkShaderModule obj() { return this->obj_; };
  [[ nodiscard ]] std::string const& name() const { return this->name_; };

private:
  ENABLE_SHARED_HELPER
};

}
