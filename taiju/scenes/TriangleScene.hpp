/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include "../scene/Scene.hpp"

namespace taiju {

class TriangleScene : public Scene {
public:
  explicit TriangleScene();

  ~TriangleScene() override = default;

public:
  bool init() override;

  bool update() override;

  void draw() override;
};

}
