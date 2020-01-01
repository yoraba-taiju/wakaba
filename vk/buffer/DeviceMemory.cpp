/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "DeviceMemory.hpp"
#include "../Vulkan.hpp"

namespace vk{

DeviceMemory::DeviceMemory(std::shared_ptr<Vulkan> const& vulkan, VkDeviceMemory vkDeviceMemory, VkDeviceSize size)
:vulkan_(vulkan)
,vkDeviceMemory_(vkDeviceMemory)
,size_(size)
{

}

DeviceMemory::~DeviceMemory() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkFreeMemory(vulkan->vkDevice(), this->vkDeviceMemory_, nullptr);
  }
}

void DeviceMemory::send(VkDeviceSize const offset, void const *src, size_t const size) {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    throw std::runtime_error("vulkan is already deleted.");
  }
  void* dst;
  VkResult result = vkMapMemory(vulkan->vkDevice(), this->vkDeviceMemory_, offset, size, 0 /* reserved */, &dst);
  if(result != VK_SUCCESS) {
    vulkan->log().fatal("Failed to map memory: {}", result);
  }
  memcpy(dst, src, size);
  vkUnmapMemory(vulkan->vkDevice(), this->vkDeviceMemory_);
}

}
