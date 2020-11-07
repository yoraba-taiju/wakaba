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
private:
  //std::vector<std::shared_ptr<Actor>> witches_;
private:
  std::shared_ptr<Sora> sora_;
  std::shared_ptr<Chitose> chitose_;
  std::shared_ptr<Momiji> momiji_;
  std::shared_ptr<Kaede> kaede_;
};

}
