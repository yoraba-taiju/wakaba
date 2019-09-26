//
// Created by kaede on 2019/09/23.
//

#pragma once

#include <GL/glew.h>
#include <memory>
#include "../../util/Shared.hpp"

namespace gl {

class TexturePool;

class Texture2D final : public std::enable_shared_from_this<Texture2D> {
private:
  GLuint const id_;
  explicit Texture2D(GLuint const id) : id_(id) {
  }
public:
  ~Texture2D();
public:
  [[ nodiscard ]] GLuint id() const {
    return this->id_;
  }
  void bind();
  void unbind() noexcept;
  static std::shared_ptr<Texture2D> create();
public:
  ENABLE_SHARED_HELPER
};

}