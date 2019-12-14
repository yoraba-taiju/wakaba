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
    vkDestroyShaderModule(vulkan->device(), this->obj_, nullptr);
  }
}

}