/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#pragma once

#include "../scene/Scene.hpp"

namespace taiju {

class StageScene final : public Scene {
public:
  bool init() override;

  bool update() override;

  void draw() override;

protected:
  ~StageScene() override = default;
};

}
