/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#include "Witch.hpp"
#include "../bullets/Bullet.hpp"

namespace taiju {

void FriendWitch::hit(std::shared_ptr<Bullet> const& bullet) {
  this->hp() -= bullet->damage();
}

}
