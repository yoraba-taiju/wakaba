/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "CommandPool.hpp"
#include "Vulkan.hpp"

namespace vk {

CommandPool::~CommandPool() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyCommandPool(vulkan->vkDevice(), vkCommandPool_, nullptr);
  }
}

}
