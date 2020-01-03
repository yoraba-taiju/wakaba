/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include "BufferBuilder.hpp"
#include "DeviceMemoryBuilder.hpp"

namespace vk {

class VertexBuffer;

class VertexBufferBuilder {
private:
  std::shared_ptr<Device> device_;
  VkDeviceSize size_;

public:
  explicit VertexBufferBuilder(std::shared_ptr<Device> device, VkDeviceSize size);

  VertexBufferBuilder(VertexBufferBuilder&&) = delete;
  VertexBufferBuilder(VertexBufferBuilder const&) = delete;
  VertexBufferBuilder& operator=(VertexBufferBuilder&&) = delete;
  VertexBufferBuilder& operator=(VertexBufferBuilder const&) = delete;
  VertexBufferBuilder() = delete;

public:
  VertexBuffer build();
};

}
