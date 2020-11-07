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
  Clock<3600> clock;
  auto value = clock.newValue<int>();
  auto one = clock.current().at();
  value = 1;
  EXPECT_EQ(1, value.get().value());
  clock.tick();
  EXPECT_EQ(1, value.get().value());
  value = 2;
  EXPECT_EQ(2, value.get().value());
  clock.leap(one);
  EXPECT_EQ(1, value.get().value());
  clock.tick();
  EXPECT_EQ(1, value.get().value());
  value = 3;
  EXPECT_EQ(3, value.get().value());
}

}
