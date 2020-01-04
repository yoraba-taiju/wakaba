/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "VertexBuffer.hpp"
#include "../builder/BufferBuilder.hpp"
#include "../util/Bridge.hpp"

namespace vk {

VertexBuffer::VertexBuffer(Buffer&& buffer)
:buffer_(std::move(buffer)){

}

void VertexBuffer::update(Bridge& bridge, size_t offset, void const* data, size_t dataSize) {
  bridge.updateBuffer(buffer_, offset, data, dataSize);
}

}
