//
// Created by kaede on 2019/12/01.
//

#include <vector>
#include "ShaderModule.hpp"
#include "Vulkan.hpp"
#include "../util/File.hpp"

namespace vk {

ShaderModule::~ShaderModule() noexcept {
  std::shared_ptr<Vulkan> vulkan = this->vulkan_.lock();
  if(vulkan) {
    vkDestroyShaderModule(vulkan->device(), vkShaderModule_, nullptr);
  }
}

std::shared_ptr<ShaderModule>
ShaderModule::create(
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
  if (vkCreateShaderModule(vulkan->device(), &shaderInfo, nullptr, &mod) != VK_SUCCESS) {
    vulkan->log().fatal("Failed to create shader module: {}", className);
  }

  std::shared_ptr<ShaderModule> shader = util::make_shared<ShaderModule>(vulkan, className, mod);
  return std::move(shader);
}

}