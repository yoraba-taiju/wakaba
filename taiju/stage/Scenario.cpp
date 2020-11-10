/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#include "Scenario.hpp"
#include "Stage.hpp"

namespace taiju {
Scenario::Scenario(std::shared_ptr<Stage> stage)
:stage_(std::move(stage))
{
}

void Scenario::init() {
}

void Scenario::move(){
  // 現在のフレーム数に応じてイベントを発生させる
  uint32_t const current = stage_->clock().current();
}

}