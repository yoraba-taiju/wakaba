//
// Created by kaede on 2019/09/23.
//

#pragma once

#include <GL/glew.h>
#include <memory>
#include "../../util/SharedUtil.hpp"

namespace gl {

class TexturePool;

class Texture2D final : public std::enable_shared_from_this<Texture2D> {
private:
  GLuint const id_;
public:
  explicit Texture2D(GLuint const id):id_(id) {
  }
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