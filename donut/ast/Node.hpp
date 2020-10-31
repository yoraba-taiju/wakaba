/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once
#include "./Position.hpp"

namespace donut {

class Node {
protected:
  inline Node(Range&& range)
  :range_(std::forward<Range>(range))
  {
  }
public:
  Node() = delete;
public:
  [[nodiscard]] Range const& range() const { return this->range_; }
private:
  Range const range_;
};

}
