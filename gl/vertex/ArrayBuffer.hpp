//
// Created by kaede on 2019/09/25.
//

#pragma once

#include <GL/glew.h>
#include "Buffer.hpp"
#include "../Util.hpp"
#include "../../util/SharedUtil.hpp"

namespace gl {

class ArrayBuffer final : public Buffer, public std::enable_shared_from_this<ArrayBuffer> {
public:
  explicit ArrayBuffer(GLuint const id): Buffer(id){}
  ~ArrayBuffer() = default;
  template <typename T>
  void set(std::vector<T> const& value) {
    Binder _(this);
    glBufferData(GL_ARRAY_BUFFER, sizeof(T), value.data(), GL_STATIC_DRAW);
  }
  void bind();
  void unbind();
  static std::shared_ptr<ArrayBuffer> create();
public:
  ENABLE_SHARED_HELPER;
};

}



