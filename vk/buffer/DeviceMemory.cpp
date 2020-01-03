/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "DeviceMemory.hpp"
#include "../Vulkan.hpp"

namespace vk{

DeviceMemory::DeviceMemory(std::shared_ptr<Device> device, VkDeviceMemory vkDeviceMemory, VkDeviceSize size)
:device_(std::move(device))
,vkDeviceMemory_(vkDeviceMemory)
,size_(size)
{

}

DeviceMemory::~DeviceMemory() noexcept {
  device_->destroyDeviceMemory(*this);
}

void DeviceMemory::sendDirect(VkDeviceSize offset, void const *src, size_t size) {
  void* dst;
  VkResult result = vkMapMemory(device_->vkDevice(), this->vkDeviceMemory_, offset, size, 0 /* reserved */, &dst);
  if(result != VK_SUCCESS) {
    throw std::runtime_error(fmt::format("Failed to map memory: {}", result));
  }
  memcpy(dst, src, size);
  vkUnmapMemory(device_->vkDevice(), this->vkDeviceMemory_);
}

}
