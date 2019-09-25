//
// Created by kaede on 2019/09/25.
//

#pragma once

#include <GL/glew.h>

namespace gl {

class Buffer {
private:
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