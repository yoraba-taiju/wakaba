/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include <vector>
#include "ShaderModule.hpp"
#include "Vulkan.hpp"
#include "../util/File.hpp"

namespace vk {

ShaderModule::~ShaderModule() noexcept {
  std::shared_ptr<Vulkan> vulkan = this->vulkan_.lock();
  if(vulkan) {
    vkDestroyShaderModule(vulkan->vkDevice(), vkShaderModule_, nullptr);
  }
}

ShaderModule ShaderModule::create(
    std::shared_ptr<Vulkan> const &vulkan,
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
  if (vkCreateShaderModule(vulkan->vkDevice(), &shaderInfo, nullptr, &mod) != VK_SUCCESS) {
    vulkan->log().fatal("Failed to create shader module: {}", className);
  }

  return ShaderModule(vulkan, className, mod);
}

}