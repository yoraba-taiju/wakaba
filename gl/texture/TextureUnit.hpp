//
// Created by kaede on 2019/09/24.
//

#pragma once

#include <memory>
#include <vector>

#include "Texture.hpp"

namespace gl{

class Texture;

class TextureUnit final {
private:
  util::Logger& log_;
  int num_;
  std::vector<std::shared_ptr<Texture>> activeTextures_;

  explicit TextureUnit(util::Logger& log);
public:
  TextureUnit() = delete;
  int bind(std::shared_ptr<Texture> const& tex);
  bool release(std::shared_ptr<Texture> const& tex);
  void activate();
  void deactivate();
  ~TextureUnit();
};

}