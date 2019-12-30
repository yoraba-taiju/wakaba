/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>

namespace util {

#define ENABLE_SHARED_HELPER template <typename X> friend struct util::shared_helper;

template<typename X>
struct shared_helper {
private:
  friend X;
  X x;
public:
  template<typename... Args>
  explicit shared_helper(Args &&... args):x(args...) {
  }

public:
  template<typename... Args>
  static std::shared_ptr<X> make_shared(Args &&... args) {
    auto p = std::make_shared<shared_helper<X>>(std::forward<Args>(args)...);
    return std::shared_ptr<X>(p, &p->x);
  }
};

template<typename X, typename... Args>
std::shared_ptr<X> make_shared(Args &&... args) {
  return shared_helper<X>::make_shared(std::forward<Args>(args)...);
}

}