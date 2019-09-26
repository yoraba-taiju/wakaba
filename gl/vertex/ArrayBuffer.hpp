//
// Created by kaede on 2019/09/25.
//

#pragma once

#include <vector>
#include <GL/glew.h>

#include "../Util.hpp"
#include "../../util/Shared.hpp"

namespace gl {

class ArrayBuffer final : public std::enable_shared_from_this<ArrayBuffer> {
private:
  static const GLenum kTarget = GL_ARRAY_BUFFER;
  GLuint const id_;
  GLenum glType_;
  std::size_t elementSize_;
  std::size_t stride_;
  std::size_t size_;
public:
  explicit ArrayBuffer(GLuint id)
  :id_(id)
  ,elementSize_(0)
  ,size_(0)
  ,stride_(0)
  ,glType_(GL_UNSIGNED_INT)
  {
  };

  ~ArrayBuffer() {
    glDeleteBuffers(1, &this->id_);
  }

  template<typename T>
  void set(std::size_t const stride, std::vector<T> const& value) {
    Binder _(this);
    this->elementSize_ = sizeof(T);
    this->size_ = value.size();
    this->stride_ = stride;
    this->glType_ = enumOf<T>();
    glBufferData(kTarget, sizeof(T) * value.size(), value.data(), GL_STATIC_DRAW);
    checkError();
  }

public:
  void bind() {
    glBindBuffer(kTarget, this->id());
    checkError();
  }

  void unbind() noexcept {
    glBindBuffer(kTarget, 0);
  }

public:
  [[nodiscard]] GLuint id() const {
    return this->id_;
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

  [[nodiscard]] std::size_t stride() const {
    return this->stride_;
  }

  static std::shared_ptr<ArrayBuffer> create() {
    GLuint id;
    glGenBuffers(1, &id);
    return util::make_shared<ArrayBuffer>(id);
  }
public:
  ENABLE_SHARED_HELPER;
};

}



