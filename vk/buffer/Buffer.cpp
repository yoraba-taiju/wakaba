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

Buffer::Buffer(std::shared_ptr<Vulkan> const& vulkan, VkBuffer vkBuffer, VkDeviceSize const size)
:vulkan_(vulkan)
,vkBuffer_(vkBuffer)
,size_(size)
{

}

Buffer::~Buffer() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyBuffer(vulkan->vkDevice(), this->vkBuffer_, nullptr);
  }
}

void Buffer::bind(std::shared_ptr<DeviceMemory> const& deviceMemory, VkDeviceSize offset) {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(!vulkan) {
    throw std::runtime_error("Vulkan is already deleted.");
  }
  std::shared_ptr<DeviceMemory> old = this->deviceMemory_;
  if(old) {
    throw std::runtime_error("Buffer is already binded.");
  }
  this->deviceMemory_ = deviceMemory;
  vkBindBufferMemory(vulkan->vkDevice(), this->vkBuffer_, deviceMemory->vkDeviceMemory(), offset);
}

}