//
// Created by kaede on 2019/09/24.
//

#pragma once

#include <memory>
#include <vector>

namespace util {
class Logger;
}

namespace gl{

class Texture2D;

class TextureUnit final {
private:
  util::Logger& log_;
  int num_;
  std::vector<std::shared_ptr<Texture2D>> activeTextures_;

  explicit TextureUnit(util::Logger& log);
public:
  TextureUnit() = delete;
  int bind(std::shared_ptr<Texture2D> const& tex);
  bool release(std::shared_ptr<Texture2D> const& tex);
  void activate();
  void deactivate();
  ~TextureUnit();
};

}