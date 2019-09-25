//
// Created by kaede on 2019/09/23.
//

#pragma once

#include <GL/glew.h>
#include <vector>
#include <memory>

#include "Texture.hpp"
#include "../../util/Logger.hpp"
#include "../../util/SharedUtil.hpp"

namespace gl {

class Texture;

class TexturePool final : public std::enable_shared_from_this<TexturePool> {
private:
  util::Logger& log_;
  std::vector<GLuint> textures_;
  explicit TexturePool(util::Logger& log);
public:
  TexturePool() = delete;
  static std::shared_ptr<TexturePool> create(util::Logger& log) {
    return util::make_shared<TexturePool>(log);
  }
  ~TexturePool();

public:
  std::shared_ptr<Texture> alloc();

  void release(GLuint handle);

private:
  ENABLE_SHARED_HELPER
};

}