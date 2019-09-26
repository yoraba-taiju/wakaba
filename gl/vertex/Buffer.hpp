//
// Created by kaede on 2019/09/25.
//

#pragma once

#include <memory>
#include <GL/glew.h>
#include "../Util.hpp"

namespace gl {

class Buffer : public std::enable_shared_from_this<Buffer> {
private:
  GLenum const target_;
  GLuint const id_;
  GLenum glType_;
  std::size_t elementSize_;
  std::size_t size_;
protected:
  explicit Buffer(GLenum target, GLuint id)
  :target_(target)
  ,id_(id)
  ,elementSize_(0)
  ,size_(0)
  {
  };

  ~Buffer() {
    glDeleteBuffers(1, &this->id_);
  }

  template<typename T>
  void setImpl(std::vector<T> const &value) {
    Binder _(this);
    this->elementSize_ = sizeof(T);
    this->size_ = value.size();
    this->glType_ = enumOf<T>();
    glBufferData(target_, sizeof(T) * value.size(), value.data(), GL_STATIC_DRAW);
    checkError();
  }

public:
  void bind() {
    glBindBuffer(target_, this->id());
    checkError();
  }

  void unbind() {
    glBindBuffer(target_, 0);
  }

public:
  [[nodiscard]] GLuint id() const {
    return this->id_;
  }

  [[nodiscard]] GLenum target() const {
    return target_;
  }

  [[nodiscard]] GLenum glType() const {
    return glType_;
  }

  [[nodiscard]] std::size_t elementSize() const {
    return this->elementSize_;
  }

  [[nodiscard]] std::size_t size() const {
    return this->size_;
  }
};

}