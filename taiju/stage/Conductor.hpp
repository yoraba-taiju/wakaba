/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#pragma once

#include <memory>
#include "Stage.hpp"

namespace taiju {

class Conductor final {

public:
  void init();
  void move();
  void draw();

private:
  std::shared_ptr<Stage> stage_;
};

}