//
// Created by kaede on 2019/09/25.
//

#pragma once

#include <GL/glew.h>
#include "Buffer.hpp"
#include "../Util.hpp"
#include "../../util/SharedUtil.hpp"

namespace gl {

template<typename T>
class ArrayBuffer final : public Buffer {
public:
  explicit ArrayBuffer(GLuint const id): Buffer(GL_ARRAY_BUFFER, id){}
  ~ArrayBuffer() = default;
  void set(std::vector<T> const& value) {
    this->setImpl(value);
  }
  static std::shared_ptr<ArrayBuffer> create() {
    GLuint id;
    glGenBuffers(1, &id);
    return util::make_shared<ArrayBuffer<T>>(id);
  }
public:
  ENABLE_SHARED_HELPER;
};

}



