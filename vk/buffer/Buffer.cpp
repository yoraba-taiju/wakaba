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

void Buffer::sendDirect(VkDeviceSize offset, void const *src, size_t size) {
  this->deviceMemory_->sendDirect(offset + this->offset_, src, size);
}

void Buffer::sendIndirect(PrimaryCommandBuffer& cmdBuffer, VkDeviceSize offset, void const *src, size_t size) {
  Buffer stagingBuffer = BufferBuilder(device_, size).setUsages(VK_BUFFER_USAGE_TRANSFER_SRC_BIT).build();
  std::shared_ptr<DeviceMemory> stagingMemory = DeviceMemoryBuilder(device_, stagingBuffer.vkMemoryRequirements(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).build();
  stagingMemory->sendDirect(0, src, size);
  stagingBuffer.bindTo(stagingMemory, 0);

  cmdBuffer.copyBufferSync(*this, offset, stagingBuffer, 0, size);
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