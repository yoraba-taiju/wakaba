//
// Created by kaede on 2019/12/01.
//

#include "Vulkan.hpp"
#include "PipelineLayout.hpp"

namespace vk {

PipelineLayout::~PipelineLayout() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyPipelineLayout(vulkan->device(), obj_, nullptr);
  }
}

}
