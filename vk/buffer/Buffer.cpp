/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Buffer.hpp"
#include "DeviceMemory.hpp"
#include "../Vulkan.hpp"
#include "../CommandPool.hpp"
#include "../builder/DeviceMemoryBuilder.hpp"
#include "../builder/BufferBuilder.hpp"
#include "../CommandBuffer.hpp"

namespace vk {

Buffer::Buffer(std::shared_ptr<Vulkan> const& vulkan, VkBuffer vkBuffer, VkDeviceSize size)
:vulkan_(vulkan)
,vkBuffer_(vkBuffer)
,deviceMemory_()
,offset_()
,size_(size)
{

}

Buffer::~Buffer() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyBuffer(vulkan->vkDevice(), this->vkBuffer_, nullptr);
  }
}

void Buffer::sendDirect(VkDeviceSize offset, void const *src, size_t size) {
  this->deviceMemory_->sendDirect(offset + this->offset_, src, size);
}

void Buffer::sendIndirect(CommandBuffer& cmdBuffer, VkDeviceSize offset, void const *src, size_t size) {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(!vulkan) {
    throw std::runtime_error("vulkan is already deleted.");
  }

  Buffer stagingBuffer = BufferBuilder(vulkan, size).setUsages(VK_BUFFER_USAGE_TRANSFER_SRC_BIT).build();
  std::shared_ptr<DeviceMemory> stagingMemory = DeviceMemoryBuilder(vulkan, stagingBuffer.vkMemoryRequirements(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).build();
  stagingMemory->sendDirect(0, src, size);
  stagingBuffer.bindTo(stagingMemory, 0);

  cmdBuffer.copyBufferSync(*this, offset, stagingBuffer, 0, size);
}

VkMemoryRequirements Buffer::vkMemoryRequirements() {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(!vulkan) {
    throw std::runtime_error("vulkan is already deleted.");
  }
  VkMemoryRequirements requirements{};
  vkGetBufferMemoryRequirements(vulkan->vkDevice(), this->vkBuffer(), &requirements);
  return requirements;
}

void Buffer::bindTo(std::shared_ptr<DeviceMemory> devMem, VkDeviceSize const offset) {
  if(this->deviceMemory_) {
    throw std::runtime_error("DeviceMemory is already bound to this buffer.");
  }
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(!vulkan) {
    throw std::runtime_error("vulkan is already deleted.");
  }
  if (vkBindBufferMemory(vulkan->vkDevice(), this->vkBuffer_, devMem->vkDeviceMemory(), offset) != VK_SUCCESS) {
    throw std::runtime_error("failed to bind buffer to device memory!");
  }
  this->deviceMemory_ = std::move(devMem);
  this->offset_ = offset;
}


}