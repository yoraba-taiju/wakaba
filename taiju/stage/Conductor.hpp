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

class Scenario;
class Conductor final {
DEF(std::shared_ptr<Stage>, stage;
DEF(std::shared_ptr<Scenario>, scenario);
public:
  Conductor(std::shared_ptr<Stage> stage, std::shared_ptr<Scenario> scenario);
  void init();
  void move();
};

}