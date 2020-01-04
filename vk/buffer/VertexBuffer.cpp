/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "VertexBuffer.hpp"
#include "../builder/BufferBuilder.hpp"

namespace vk {

VertexBuffer::VertexBuffer(Buffer&& buffer)
:buffer_(std::move(buffer)){

}

void VertexBuffer::update(PrimaryCommandBuffer &cmdBuffer, size_t offset, void const* data, size_t dataSize) {
  buffer_.sendIndirect(cmdBuffer, offset, data, dataSize);
}

}
