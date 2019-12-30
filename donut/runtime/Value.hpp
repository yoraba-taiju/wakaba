/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <array>
#include <optional>
#include <memory>
#include <utility>
#include <algorithm>

namespace donut::runtime {

using Time = uint32_t;

class Clock {
public:
  explicit Clock(Time const now) : now_(now) {
  }

  Clock() : Clock(0) {
  }

  virtual ~Clock() = default;

public:
  void tick() {
    this->now_++;
  }

  Time leap(Time const &now) {
    this->now_ = now;
    return now;
  }

  [[nodiscard]] virtual Time now() const {
    return this->now_;
  }

private:
  Time now_;
};

class ShiftedClock final : Clock {
public:
  explicit ShiftedClock(std::shared_ptr<Clock> parent, Time const delta)
      : parent_(std::move(parent)), delta_(delta) {
  }

  ~ShiftedClock() override = default;

public:
  [[nodiscard]] Time now() const override {
    return this->parent_->now() + this->delta_;
  }

private:
  Time delta_;
  std::shared_ptr<Clock> parent_;
};

template<typename Type, std::size_t length>
class Value final {
public:
  explicit Value(const Clock &clock) : clock_(clock) {

  }

private:
  Type const &set(Value const &v) {
    Time const t = clock_.now();
    std::tuple<Time, Type> &last = values_.back();
    if (std::get<0>(last) == t) {
      std::get<1>(last) = v;
    } else {
      typename std::array<std::tuple<Time, Type>, length>::const_iterator it = this->findReadEntry();
      if (it == this->values_.cend()) {
      }
      return std::get<1>(*it);
    }
    return v;
  }

  std::optional<Type const &> const &get() const {
    return this->peek(this->clock_.now());
  }

  std::optional<Type const &> peek(Time const t) const {
    std::tuple<Time, Type> &last = values_.back();
    if (std::get<0>(last) == t) {
      return std::get<1>(last);
    } else {
      typename std::array<std::tuple<Time, Type>, length>::const_iterator it = this->findReadEntry();
      if (it == this->values_.cend()) {
        return std::nullopt;
      }
      return std::get<1>(*it);
    }
  }

  struct ValueEntryLessComparator final {
    bool operator()(std::tuple<Time, Type> const &a, std::tuple<Time, Type> const &b) {
      return std::get<0>(a) < std::get<0>(b);
    }
  };

  typename std::array<std::tuple<Time, Type>, length>::const_iterator findReadEntry(Time const &t) const {
    std::upper_bound(values_.cbegin(), values_.cend(), ValueEntryLessComparator());
  }

  typename std::array<std::tuple<Time, Type>, length>::iterator findWriteEntry(Time const &t) {
    return std::lower_bound(values_.begin(), values_.end(), ValueEntryLessComparator());
  }

private:
  Clock clock_;
  std::array<std::tuple<Time, Type>, length> values_;
};

}