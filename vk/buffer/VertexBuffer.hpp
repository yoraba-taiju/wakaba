/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include "Buffer.hpp"

namespace vk {
class Bridge;

class VertexBuffer final {
private:
  Buffer buffer_;
public:
  explicit VertexBuffer(Buffer&& buffer);
  VertexBuffer() = delete;
  VertexBuffer(VertexBuffer const&) = delete;
  VertexBuffer(VertexBuffer&&) = default;
  VertexBuffer& operator=(VertexBuffer const&) = delete;
  VertexBuffer& operator=(VertexBuffer&&) = default;
  ~VertexBuffer() = default;
public:
  [[ nodiscard ]] Buffer& buffer() {
    return buffer_;
  }
  [[ nodiscard ]] size_t size() {
    return static_cast<size_t>(this->buffer().size());
  }

public:
  void update(Bridge& bridge, size_t offset, void const* data, size_t dataSize);
  template <typename T>
  void update(Bridge& bridge, size_t offset, std::vector<T> const& data, size_t dataFromIndex = 0, std::optional<size_t> count = std::optional<size_t>()) {
    void const* dataBegin = std::next(data.data(), dataFromIndex);
    size_t sizeInBytes = (count.has_value() ? count.value() : (data.size() - dataFromIndex)) * sizeof(T);
    update(bridge, offset, dataBegin, sizeInBytes);
  }
  template <typename T>
  void update(Bridge& bridge, std::vector<T> const& data) {
    update<T>(bridge, 0, data, 0, std::make_optional(data.size()));
  }
};

}


