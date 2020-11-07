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

template <size_t length> class Clock;
template <typename Type, size_t length> class Value;

class SubjectiveTime final {
  template <size_t length> friend class Clock;
public:
  SubjectiveTime() = default;
  SubjectiveTime(SubjectiveTime const&) = default;
  SubjectiveTime(SubjectiveTime&&) = default;
  SubjectiveTime& operator=(SubjectiveTime const&) = default;
  SubjectiveTime& operator=(SubjectiveTime&&) = default;
public:
  [[ nodiscard ]] uint32_t leap() const { return this->leap_; }
  [[ nodiscard ]] uint32_t at() const { return this->at_; }
public:
  bool operator< (SubjectiveTime const& rhs) const {
    if(this->at_ == rhs.at_) {
      return this->leap_ < rhs.leap_;
    }
    return this->leap_ < rhs.leap_;
  }
  bool operator<= (SubjectiveTime const& rhs) const {
    if(this->at_ == rhs.at_) {
      return this->leap_ <= rhs.leap_;
    }
    return this->leap_ < rhs.leap_;
  }
  bool operator== (SubjectiveTime const& rhs) const {
    return (this->leap_ == rhs.leap_) && (this->at_ == rhs.at_);
  }
  bool operator> (SubjectiveTime const& rhs) const {
    return !(*this <= rhs);
  }
  bool operator>= (SubjectiveTime const& rhs) const {
    return !(*this < rhs);
  }
private:
  uint32_t leap_{};
  uint32_t at_{};
};

template <size_t length> class Clock {
public:
  explicit Clock() = default;

  Clock(Clock const&) = delete;
  Clock operator=(Clock const&) = delete;

  Clock(Clock<length>&&)  noexcept = default;
  Clock& operator=(Clock<length>&&)  noexcept = default;

  ~Clock() noexcept = default;

public:
  void tick() {
    subjectiveTime_.at_++;
  }

  SubjectiveTime const& leap(uint32_t const to) {
    subjectiveTime_.leap_++;
    subjectiveTime_.at_ = to;
    this->branches_.emplace_back(to);
    auto const currentLeap = subjectiveTime_.leap_;
    while((currentLeap - branchHorizon_) > length) {
      branchHorizon_++;
      branches_.pop_front();
    }
    for(auto it = this->branches_.begin(); it != branches_.end(); ++it) {
      *it = std::min(*it, to);
    }
    return subjectiveTime_;
  }

  [[nodiscard]] SubjectiveTime const& current() const {
    return this->subjectiveTime_;
  }

  [[nodiscard]] uint32_t branchTimeOf(uint32_t const leap) const {
    return leap < this->branchHorizon_ ? UINT32_MAX : this->branches_[leap - this->branchHorizon_];
  }

  template<typename T>
  [[nodiscard]] Value<T, length> newValue() {
    return Value<T, length>(*this);
  }

private:
  SubjectiveTime subjectiveTime_{};
private:
  std::deque<uint32_t> branches_ = {};
  std::size_t branchHorizon_ = 0;
};

template <size_t length> class ShiftedClock final : Clock<length> {
public:
  explicit ShiftedClock(std::shared_ptr<Clock<length>> parent, SubjectiveTime const delta)
      : parent_(std::move(parent)), delta_(delta) {
  }
  ~ShiftedClock() = default;
public:
  [[nodiscard]] SubjectiveTime now() const {
    return this->parent_->current();// FIXME + this->delta_;
  }

private:
  SubjectiveTime delta_;
  std::shared_ptr<Clock<length>> parent_;
};

template<typename Type, std::size_t length>
class Value final {
public:
  Value() = delete;
  Value(Value const&) = delete;
  Value& operator=(Value const&) = delete;
  explicit Value(Clock<length>& clock)
  :clock_(clock)
  ,beg_(0)
  ,end_(0)
  ,lastModifiedLeap_(clock.current().leap())
  {
  }

public:
  Value<Type, length>& operator=(Type&& v) {
    this->set(std::forward<Type>(v));
    return *this;
  }

  Optional<Type const> get() const {
    return this->peek(this->clock_.current());
  }

private:
  Value<Type, length>& set(Type&& v) {
    SubjectiveTime const t = clock_.current();
    auto& last = values_[end_-1];
    if (beg_ != end_ && std::get<0>(last) == t.at() && lastModifiedLeap_ <= t.leap()) {
      std::get<1>(last) = std::forward<Type>(v);
    } else {
      size_t idx = this->findEntry(t);
      values_[idx] = std::make_tuple(t.at(), std::forward<Type>(v));
      if((end_+1)%length == beg_) {
        beg_++;
      }
      end_ = idx + 1;
    }
    this->lastModifiedLeap_ = t.leap();
    return *this;
  }

  Optional<Type const> peek(SubjectiveTime const t) const {
    auto const& last = values_[end_-1];
    if (beg_ != end_ && std::get<0>(last) == t.at() && lastModifiedLeap_ <= t.leap()) {
      return Optional<Type const>(std::get<1>(last));
    } else {
      auto const idx = this->findEntry(t);
      if (idx == end_) {
        return Optional<Type const>();
      }
      return Optional<Type const>(std::get<1>(values_[idx]));
    }
  }

  [[nodiscard]] size_t findEntry(SubjectiveTime const& subjectiveTime) const {
    size_t beg = beg_;
    size_t end = end_;
    if(beg > end) {
      end = end + length;
    }
    uint32_t const t = std::min(clock_.branchTimeOf(subjectiveTime.leap()), subjectiveTime.at());
    while(beg < end) {
      size_t mid = beg + (end-beg)/2;
      uint32_t const midTime = std::get<0>(values_[mid % length]);
      if (midTime > t) {
        end = mid - 1;
      } else if  (midTime < t) {
        beg = mid + 1;
      } else {
        return mid % length;
      }
    }
    return beg % length;
  }

private:
  Clock<length>& clock_;
  std::array<std::tuple<uint32_t, Type>, length> values_;
  size_t beg_ = {};
  size_t end_ = {};
  uint32_t lastModifiedLeap_ = {};
};

}