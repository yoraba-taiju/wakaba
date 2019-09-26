//
// Created by kaede on 2019/09/24.
//

#pragma once

#include <memory>
#include <vector>
#include "../../util/SharedUtil.hpp"

namespace util {
class Logger;
}

namespace gl{

class Texture2D;

class TextureUnit final {
private:
  int num_;
  std::vector<std::shared_ptr<Texture2D>> activeTextures_;
  TextureUnit();

public:
  ~TextureUnit() noexcept;

public:
  int bind(std::shared_ptr<Texture2D> const& tex);
  bool release(std::shared_ptr<Texture2D> const& tex);

public:
  void activate();
  void deactivate();

public:
  static std::shared_ptr<TextureUnit> create();
public:
  ENABLE_SHARED_HELPER
};

}