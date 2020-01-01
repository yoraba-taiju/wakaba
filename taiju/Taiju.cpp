/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Taiju.hpp"

namespace taiju {

Taiju::Taiju(util::Logger &log)
:log_(log) {

}

bool Taiju::update() {
  if (!this->currentScene_) {
    return false;
  }
  return this->currentScene_->update();
}

void Taiju::draw() {
  if (this->currentScene_) {
    this->currentScene_->draw();
  }
}

}