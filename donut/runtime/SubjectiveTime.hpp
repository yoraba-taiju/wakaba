/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <array>
#include <deque>
#include <memory>
#include <utility>
#include <algorithm>

#include "Optional.hpp"

namespace donut {

template<size_t length>
class Clock;

class SubjectiveTime final {
  template<size_t length> friend
  class Clock;

public:
  SubjectiveTime() = default;

  SubjectiveTime(SubjectiveTime const &) = default;

  SubjectiveTime(SubjectiveTime &&) = default;

  SubjectiveTime &operator=(SubjectiveTime const &) = default;

  SubjectiveTime &operator=(SubjectiveTime &&) = default;

public:
  [[nodiscard]] uint32_t leap() const { return this->leap_; }

  [[nodiscard]] uint32_t at() const { return this->at_; }

public:
  bool operator<(SubjectiveTime const &rhs) const {
    if (this->at_ == rhs.at_) {
      return this->leap_ < rhs.leap_;
    }
    return this->leap_ < rhs.leap_;
  }

  bool operator<=(SubjectiveTime const &rhs) const {
    if (this->at_ == rhs.at_) {
      return this->leap_ <= rhs.leap_;
    }
    return this->leap_ < rhs.leap_;
  }

  bool operator==(SubjectiveTime const &rhs) const {
    return (this->leap_ == rhs.leap_) && (this->at_ == rhs.at_);
  }

  bool operator>(SubjectiveTime const &rhs) const {
    return !(*this <= rhs);
  }

  bool operator>=(SubjectiveTime const &rhs) const {
    return !(*this < rhs);
  }

private:
  uint32_t leap_{};
  uint32_t at_{};
};

}