//
// Created by kaede on 2019/10/14.
//

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
