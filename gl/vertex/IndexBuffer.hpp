//
// Created by kaede on 2019/09/26.
//

#pragma once

#include <vector>
#include <GL/glew.h>

#include "../Util.hpp"
#include "../../util/Shared.hpp"

namespace gl {

class IndexBuffer final : public std::enable_shared_from_this<IndexBuffer> {
private:
  static GLenum const kTarget = GL_ELEMENT_ARRAY_BUFFER;
  GLuint const id_;
  GLenum glType_;
  std::size_t elementSize_;
  std::size_t size_;

private:
  explicit IndexBuffer(GLuint const id)
  :id_(id)
  ,elementSize_(0)
  ,size_(0)
  ,glType_(GL_UNSIGNED_INT)
  {
  }

public:
  ~IndexBuffer() {
    glDeleteBuffers(1, &this->id_);
  }

public:
  template<typename T>
  void set(std::vector<T> const &value) {
    Binder _(this);
    this->elementSize_ = sizeof(T);
    this->size_ = value.size();
    this->glType_ = enumOf<T>();
    glBufferData(kTarget, sizeof(T) * value.size(), value.data(), GL_STATIC_DRAW);
    checkError();
  }

  void bind() {
    glBindBuffer(kTarget, this->id());
    checkError();
  }

  void unbind() {
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

public:
  static std::shared_ptr<IndexBuffer> create();
  void draw();
public:
  ENABLE_SHARED_HELPER;
};

}