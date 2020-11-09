/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#pragma once

#include "../../util/Prop.hpp"
#include "../../donut/runtime/Value.hpp"

namespace taiju {
template <typename T> using Value = donut::Value<T, 3600>;
}
