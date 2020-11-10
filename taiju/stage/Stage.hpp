/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#pragma once
#include <vector>
#include <memory>
#include "witches/Witch.hpp"
#include "witches/Sora.hpp"
#include "witches/Chitose.hpp"
#include "witches/Momiji.hpp"
#include "witches/Kaede.hpp"

namespace taiju {

class Stage {
DEF(Clock, clock);
DEF(std::vector<std::shared_ptr<Actor>>, actors);
DEF(std::shared_ptr<Sora>, sora);
DEF(std::shared_ptr<Chitose>, chitose);
DEF(std::shared_ptr<Momiji>, momiji);
DEF(std::shared_ptr<Kaede>, kaede);
public:
  Stage() = default;
  Stage(Stage const&) = delete;
  Stage(Stage&&) = delete;
  Stage& operator=(Stage const&) = delete;
  Stage& operator=(Stage&&) = delete;

public:
  void init();
};

}
