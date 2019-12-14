//
// Created by kaede on 2019/11/18.
//

#include "CommandPool.hpp"
#include "Vulkan.hpp"

namespace vk {

CommandPool::~CommandPool() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyCommandPool(vulkan->device(), obj_, nullptr);
  }
}

}
