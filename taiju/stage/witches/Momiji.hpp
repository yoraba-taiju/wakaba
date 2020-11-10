/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#pragma once

#include <cstdint>
#include "Witch.hpp"
#include "../Value.hpp"

namespace taiju {

class Stage;
class Momiji final : public Witch {
VDEF(int32_t, hp);
public:
  void move(std::shared_ptr<Stage>);
};

}
