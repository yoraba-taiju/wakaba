/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#include "Conductor.hpp"

namespace taiju {

Conductor::Conductor(std::shared_ptr<Stage> stage, std::shared_ptr<Scenario> scenario)
:stage_(std::move(stage))
,scenario_(std::move(scenario))
{
}

void Conductor::init() {
}

void Conductor::move() {
}

}