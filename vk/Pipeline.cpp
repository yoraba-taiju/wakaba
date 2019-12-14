//
// Created by kaede on 2019/12/14.
//

#include "Vulkan.hpp"
#include "Pipeline.hpp"

namespace vk {

Pipeline::~Pipeline() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyPipeline(vulkan->device(), this->obj_, nullptr);
  }
}

}
