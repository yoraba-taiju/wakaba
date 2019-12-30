/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Vulkan.hpp"
#include "PipelineLayout.hpp"

namespace vk {

PipelineLayout::~PipelineLayout() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyPipelineLayout(vulkan->vkDevice(), vkPipelineLayout_, nullptr);
  }
}

}
