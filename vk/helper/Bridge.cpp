/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Bridge.hpp"
#include "../Device.hpp"
#include "../builder/BufferBuilder.hpp"
#include "../builder/DeviceMemoryBuilder.hpp"
#include "../buffer/Buffer.hpp"
#include "../buffer/DeviceMemory.hpp"
#include "../command/CommandPool.hpp"
#include "../command/PrimaryCommandBuffer.hpp"

namespace vk {

Bridge::Bridge(std::shared_ptr<Device> device)
:device_(std::move(device))
,commandPoolMutex_(std::make_unique<std::mutex>())
,stagingBufferReleaser_(*this)
,stagingBufferMutex_(std::make_unique<std::mutex>())
{

}

std::shared_ptr<CommandPool> Bridge::commandPool() {
  std::lock_guard<std::mutex> lock(*this->commandPoolMutex_);
  std::thread::id id = std::thread::id();
  auto it = this->commandPools_.find(id);
  if(it != this->commandPools_.end()) {
    return it->second;
  }
  std::shared_ptr<CommandPool> pool = device_->createCommandPool();
  this->commandPools_.emplace(id, pool);
  return std::move(pool);
}

std::unique_ptr<Buffer, Bridge::StagingBufferReleaser> Bridge::stagingBuffer() {
  std::lock_guard<std::mutex> lock(*this->stagingBufferMutex_);
  if(!this->stagingBuffer_.empty()) {
    std::unique_ptr<Buffer> buff = std::move(this->stagingBuffer_.back());
    this->stagingBuffer_.pop_back();
    return std::unique_ptr<Buffer, Bridge::StagingBufferReleaser>(buff.release(), this->stagingBufferReleaser_);
  }

  Buffer stagingBuffer = BufferBuilder(device_, StagingBufferSize).setUsages(VK_BUFFER_USAGE_TRANSFER_SRC_BIT).build();
  std::shared_ptr<DeviceMemory> stagingMemory = DeviceMemoryBuilder(device_, stagingBuffer.vkMemoryRequirements(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).build();
  stagingBuffer.bindTo(stagingMemory, 0);

  std::unique_ptr<Buffer> buff = std::make_unique<Buffer>(std::move(stagingBuffer));
  return std::unique_ptr<Buffer, StagingBufferReleaser>(buff.release(), this->stagingBufferReleaser_);
}

void Bridge::StagingBufferReleaser::operator()(Buffer* buff) const {
  std::lock_guard<std::mutex> lock(*this->bridge_.stagingBufferMutex_);
  this->bridge_.stagingBuffer_.emplace_back(std::unique_ptr<Buffer>(buff));
}

void Bridge::updateBuffer(Buffer &buffer, VkDeviceSize offset, void const *src, size_t size) {
  std::shared_ptr<CommandPool> pool = this->commandPool();
  if(size > StagingBufferSize) {
    Buffer stagingBuffer = BufferBuilder(device_, StagingBufferSize).setUsages(VK_BUFFER_USAGE_TRANSFER_SRC_BIT).build();
    std::shared_ptr<DeviceMemory> stagingMemory = DeviceMemoryBuilder(device_, stagingBuffer.vkMemoryRequirements(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).build();
    stagingMemory->sendDirect(0, src, size);
    stagingBuffer.bindTo(stagingMemory, 0);
    pool->createPrimaryBuffer().copyBufferSync(buffer, offset, stagingBuffer, 0, size);
    return;
  }
  std::unique_ptr<Buffer, Bridge::StagingBufferReleaser> stagingBuffer = this->stagingBuffer();
  stagingBuffer->deviceMemory()->sendDirect(0, src, size);
  pool->createPrimaryBuffer().copyBufferSync(buffer, offset, *stagingBuffer, 0, size);
}



}
