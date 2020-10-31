/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */

#include <gtest/gtest.h>
#include "./Stream.hpp"

namespace donut {

TEST(DonutStreamTest, StreamTest) {
  auto stream = Stream::open("resources/test/simple.txt");
  EXPECT_EQ(stream.content(), "OK");
}

}