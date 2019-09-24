//
// Created by kaede on 2019/09/23.
//

#pragma once

#include <GL/glew.h>
#include <memory>
#include "TexturePool.hpp"

namespace gl {

class TexturePool;

class Texture final {
private:
  std::shared_ptr<TexturePool> const pool_;
  GLuint id_;
public:
  Texture(std::shared_ptr<TexturePool> pool, GLuint handle)
      : pool_(std::move(pool)), id_(handle) {
  }

  ~Texture();

public:
  [[ nodiscard ]] GLuint id() const {
    return this->id_;
  }
};

}