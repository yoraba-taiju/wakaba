//
// Created by kaede on 2019/09/27.
//

#pragma once

#include <GL/glew.h>

#include "../Util.hpp"
#include "../../util/Shared.hpp"


class VertexArray final : public std::enable_shared_from_this<VertexArray> {
private:
  GLuint const id_;
public:
  explicit VertexArray(GLuint id)
  :id_(id)  {
  };

  ~VertexArray() {
    glDeleteVertexArrays(1, &this->id_);
  }

public:
  void bind() {
    glBindVertexArray(this->id_);
  }

  void unbind() noexcept {
    glBindVertexArray(0);
  }

public:
  [[nodiscard]] GLuint id() const {
    return this->id_;
  }

  static std::shared_ptr<VertexArray> create() {
    GLuint id;
    glGenVertexArrays(1, &id);
    return util::make_shared<VertexArray>(id);
  }
public:
  ENABLE_SHARED_HELPER;
};

