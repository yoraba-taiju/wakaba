/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include <vector>
#include <fmt/format.h>
#include "Device.hpp"
#include "ShaderModule.hpp"

namespace vk {

ShaderModule::~ShaderModule() noexcept {
  if(device_){
    device_->destroyShaderModule(*this);
  }
}

ShaderModule ShaderModule::create(
    std::shared_ptr<Device> const& device,
    uint32_t const *code, size_t length,
    const char *className) noexcept(false) {

  VkShaderModuleCreateInfo shaderInfo{
      .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .codeSize = length,
      .pCode = code,
  };

  VkShaderModule mod{};
  if (vkCreateShaderModule(device->vkDevice(), &shaderInfo, nullptr, &mod) != VK_SUCCESS) {
    throw std::runtime_error(fmt::format("Failed to create shader module: {}", className));
  }

  return ShaderModule(device, className, mod);
}

}