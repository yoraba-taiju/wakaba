/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#pragma once
#include "Value.hpp"

namespace taiju {

class Stage;
class Scenario {
DEF(std::shared_ptr<Stage>, stage);
public:
  explicit Scenario(std::shared_ptr<Stage> stage);
  void init();
  void move();
};

}
