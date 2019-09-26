//
// Created by kaede on 2019/09/25.
//

#pragma once

#include <memory>
#include <GL/glew.h>

namespace gl {

class BufferPool;

class Buffer {
private:
  std::shared_ptr<BufferPool> const pool_;
  GLuint const id_;
protected:
  explicit Buffer(GLuint id);
  ~Buffer();
public:
  [[nodiscard]] GLuint id() const {
    return this->id_;
  }
};

}