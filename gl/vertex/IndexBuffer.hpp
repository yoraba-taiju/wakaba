//
// Created by kaede on 2019/09/26.
//

#pragma once

#include <vector>
#include <GL/glew.h>
#include "Buffer.hpp"
#include "../Util.hpp"
#include "../../util/SharedUtil.hpp"

namespace gl {

class IndexBuffer final : public Buffer<GL_ELEMENT_ARRAY_BUFFER>, public std::enable_shared_from_this<IndexBuffer> {
private:
  size_t length_;
public:
  explicit IndexBuffer(GLuint const id): Buffer<GL_ELEMENT_ARRAY_BUFFER>(id){}
  ~IndexBuffer() = default;

public:
  void set(std::vector<uint16_t> const& value);
  static std::shared_ptr<IndexBuffer> create();
  void draw();
public:
  ENABLE_SHARED_HELPER;
};

}