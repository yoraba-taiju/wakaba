//
// Created by kaede on 2019/09/25.
//

#pragma once

#include <memory>
#include <GL/glew.h>
#include "../Util.hpp"

namespace gl {

template<GLenum target>
class Buffer {
private:
  GLuint const id_;
  std::size_t elementSize_;
  std::size_t size_;
protected:
  explicit Buffer(GLuint id) : id_(id), elementSize_(0), size_(0) {

  };

  ~Buffer() {
    glDeleteBuffers(1, &this->id_);
  }

  template<typename T>
  void setImpl(std::vector<T> const &value) {
    Binder _(this);
    glBufferData(target, sizeof(T) * value.size(), value.data(), GL_STATIC_DRAW);
    checkError();
  }

public:
  void bind() {
    glBindBuffer(target, this->id());
    checkError();
  }

  void unbind() {
    glBindBuffer(target, 0);
  }

public:
  [[nodiscard]] GLuint id() const {
    return this->id_;
  }

  [[nodiscard]] std::size_t elementSize() const {
    return this->elementSize_;
  }

  [[nodiscard]] std::size_t size() const {
    return this->size_;
  }
};

}