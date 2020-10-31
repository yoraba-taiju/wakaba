/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>
#include "scene/Scene.hpp"
#include "../util/Logger.hpp"

namespace taiju {

class Taiju final {
private:
  util::Logger& log_;
  std::shared_ptr<Scene> currentScene_;
public:
  explicit Taiju(util::Logger &log);
  Taiju() = delete;
  Taiju(Taiju const&) = delete;
  Taiju(Taiju&&) = delete;
  Taiju& operator=(Taiju const&) = delete;
  Taiju& operator=(Taiju&&) = delete;

public:
  bool update();

  void draw();
};

int main();

}