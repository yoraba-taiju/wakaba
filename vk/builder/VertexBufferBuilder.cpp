/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "VertexBufferBuilder.hpp"

#include <utility>
#include "../buffer/VertexBuffer.hpp"

namespace vk {

VertexBufferBuilder::VertexBufferBuilder(std::shared_ptr<Device> device, VkDeviceSize size)
:device_(std::move(device))
,size_(size)
{
}

VertexBuffer VertexBufferBuilder::build() {
  BufferBuilder bufferBuilder(device_, size_);
  bufferBuilder.setUsages(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
  Buffer buff = bufferBuilder.build();

  DeviceMemoryBuilder devMemBuilder(device_, buff.vkMemoryRequirements(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  std::shared_ptr<DeviceMemory> devMem = devMemBuilder.build();

  buff.bindTo(devMem, 0);

  return VertexBuffer(std::move(buff));
}

}