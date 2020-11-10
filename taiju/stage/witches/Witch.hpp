/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#pragma once

#include "../Geom.hpp"
#include "../Actor.hpp"
#include "../Value.hpp"

namespace taiju {

class Witch : public Actor {
VDEF_RW(Pos, pos, public, protected);
public:
  static constexpr float kRadius = 1;
  [[nodiscard]] float radius() const {
    return kRadius;
  }
};

}
