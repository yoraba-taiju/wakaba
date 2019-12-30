/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

namespace taiju {

class Scene {
protected:
  Scene() = default;

  virtual ~Scene() = default;

public:
  virtual bool init() = 0;

  virtual bool update() = 0;

  virtual void draw() = 0;
};

}

