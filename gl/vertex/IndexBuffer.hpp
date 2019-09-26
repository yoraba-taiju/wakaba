//
// Created by kaede on 2019/09/26.
//

#pragma once

#include <GL/glew.h>
#include "Buffer.hpp"
#include "../Util.hpp"
#include "../../util/SharedUtil.hpp"

namespace gl {

class IndexBuffer final : public Buffer, public std::enable_shared_from_this<IndexBuffer> {
public:
  explicit IndexBuffer(GLuint const id): Buffer(id){}
  ~IndexBuffer() = default;

public:
  template <typename T>
  void set(std::vector<T> const& value) {
    Binder _(this);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T), value.data(), GL_STATIC_DRAW);
  }
  void bind();
  void unbind();
  static std::shared_ptr<IndexBuffer> create();
public:
  ENABLE_SHARED_HELPER;
};

}