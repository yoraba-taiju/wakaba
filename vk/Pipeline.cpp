/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Vulkan.hpp"
#include "Pipeline.hpp"

namespace vk {

Pipeline::~Pipeline() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyPipeline(vulkan->vkDevice(), vkPipeline_, nullptr);
  }
}

}
