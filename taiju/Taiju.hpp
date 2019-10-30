//
// Created by kaede on 2019/09/23.
//

#pragma once

#include <memory>
#include "scene/Scene.hpp"
#include "../util/Logger.hpp"

namespace taiju {

class Taiju final {
private:
  util::Logger log_;
  std::shared_ptr<Scene> currentScene_;
public:
  explicit Taiju(util::Logger& log);

  bool update();

  void draw();
};

}