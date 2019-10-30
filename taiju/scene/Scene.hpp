//
// Created by kaede on 2019/10/14.
//

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

