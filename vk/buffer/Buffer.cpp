/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Buffer.hpp"
#include "DeviceMemory.hpp"
#include "../Device.hpp"
#include "../command/CommandPool.hpp"
#include "../builder/DeviceMemoryBuilder.hpp"
#include "../builder/BufferBuilder.hpp"
#include "../command/PrimaryCommandBuffer.hpp"

namespace vk {

Buffer::Buffer(std::shared_ptr<Device> device, VkBuffer vkBuffer, VkDeviceSize size)
:device_(std::move(device))
,vkBuffer_(vkBuffer)
,deviceMemory_()
,offset_()
,size_(size)
{

}

Buffer::~Buffer() noexcept {
  if(device_) {
    device_->destroyBuffer(*this);
  }
}

VkMemoryRequirements Buffer::vkMemoryRequirements() {
  VkMemoryRequirements requirements{};
  vkGetBufferMemoryRequirements(device_->vkDevice(), this->vkBuffer(), &requirements);
  return requirements;
}

void Buffer::bindTo(std::shared_ptr<DeviceMemory> devMem, VkDeviceSize const offset) {
  if(this->deviceMemory_) {
    throw std::runtime_error("DeviceMemory is already bound to this buffer.");
  }
  if (vkBindBufferMemory(device_->vkDevice(), this->vkBuffer_, devMem->vkDeviceMemory(), offset) != VK_SUCCESS) {
    throw std::runtime_error("failed to bind buffer to device memory!");
  }
  this->deviceMemory_ = std::move(devMem);
  this->offset_ = offset;
}


}