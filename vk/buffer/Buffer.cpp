/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Buffer.hpp"
#include "../Vulkan.hpp"
#include "DeviceMemory.hpp"

namespace vk {

Buffer::Buffer(std::shared_ptr<Vulkan> const& vulkan, VkBuffer vkBuffer, std::shared_ptr<DeviceMemory> deviceMemory, VkDeviceSize offset, VkDeviceSize size)
:vulkan_(vulkan)
,vkBuffer_(vkBuffer)
,deviceMemory_(std::move(deviceMemory))
,offset_(offset)
,size_(size)
{

}

Buffer::~Buffer() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyBuffer(vulkan->vkDevice(), this->vkBuffer_, nullptr);
  }
}

void Buffer::send(VkDeviceSize offset, void const *src, size_t size) {
  this->deviceMemory_->send(offset + this->offset_, src, size);
}

}