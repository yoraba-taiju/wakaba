/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */

#include <gtest/gtest.h>
#include "./Value.hpp"

namespace donut {

TEST(DonutValueTest, SimpleTest) {
  Clock clock;
  auto value = clock.newValue<int, 3600>();
  value = 1;
  EXPECT_EQ(1, value.get());
}

}