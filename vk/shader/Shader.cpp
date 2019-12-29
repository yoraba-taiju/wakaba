//
// Created by kaede on 2019/12/29.
//

#include "Shader.hpp"

namespace vk {

Shader::~Shader() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
  }
}

}
