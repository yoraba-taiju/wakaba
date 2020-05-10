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

class Clock;

class SubjectiveTime final {
  friend class Clock;
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
    if(this->leap_ == rhs.leap_) {
      return this->at_ < rhs.at_;
    }
    return this->leap_ < rhs.leap_;
  }
  bool operator<= (SubjectiveTime const& rhs) const {
    if(this->leap_ == rhs.leap_) {
      return this->at_ <= rhs.at_;
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

class Clock {
public:
  explicit Clock() = default;

  Clock(Clock const&) = delete;
  Clock operator=(Clock const&) = delete;

  Clock(Clock&&) = default;
  Clock& operator=(Clock&&) = default;

  virtual ~Clock();

public:
  void tick() {
    subjectiveTime_.at_++;
  }

  SubjectiveTime const& leap(uint32_t const to) {
    subjectiveTime_.leap_++;
    subjectiveTime_.at_ = to;
    return subjectiveTime_;
  }

  [[nodiscard]] virtual SubjectiveTime const& current() const {
    return this->subjectiveTime_;
  }

private:
  SubjectiveTime subjectiveTime_{};
};

class ShiftedClock final : Clock {
public:
  explicit ShiftedClock(std::shared_ptr<Clock> parent, SubjectiveTime const delta)
      : parent_(std::move(parent)), SubjectiveTime(delta) {
  }

  ~ShiftedClock() override = default;

public:
  [[nodiscard]] SubjectiveTime now() const override {
    return this->parent_->now() + this->delta_;
  }

private:
  SubjectiveTime delta_;
  std::shared_ptr<Clock> parent_;
};

template<typename Type, std::size_t length>
class Value final {
public:
  Value() = default;

  Value(Value const&) = delete;
  Value& operator=(Value const&) = delete;

  explicit Value(Clock* clock) : clock_(clock) {

  }

private:
  Value& operator=(Type const &v) {
    return this->set(v);
  }

  Type& set(Type const &v) {
    SubjectiveTime const t = clock_.current();
    std::tuple<SubjectiveTime, Type> &last = values_.back();
    if (std::get<0>(last) == t) {
      std::get<1>(last) = v;
    } else {
      typename std::array<std::tuple<SubjectiveTime, Type>, length>::const_iterator it = this->findReadEntry();
      if (it == this->values_.cend()) {
      }
      return std::get<1>(*it);
    }
    return v;
  }

  std::optional<Type const &> const &get() const {
    return this->peek(this->clock_.now());
  }

  std::optional<Type const &> peek(SubjectiveTime const t) const {
    std::tuple<SubjectiveTime, Type> &last = values_.back();
    if (std::get<0>(last) == t) {
      return std::get<1>(last);
    } else {
      typename std::array<std::tuple<SubjectiveTime, Type>, length>::const_iterator it = this->findReadEntry();
      if (it == this->values_.cend()) {
        return std::nullopt;
      }
      return std::get<1>(*it);
    }
  }

  struct ValueEntryLessComparator final {
    bool operator()(std::tuple<SubjectiveTime, Type> const &a, std::tuple<SubjectiveTime, Type> const &b) {
      return std::get<0>(a) < std::get<0>(b);
    }
  };

  typename std::array<std::tuple<SubjectiveTime, Type>, length>::const_iterator findReadEntry(SubjectiveTime const &t) const {
    std::upper_bound(values_.cbegin(), values_.cend(), ValueEntryLessComparator());
  }

  typename std::array<std::tuple<SubjectiveTime, Type>, length>::iterator findWriteEntry(SubjectiveTime const &t) {
    return std::lower_bound(values_.begin(), values_.end(), ValueEntryLessComparator());
  }

private:
  Clock clock_;
  std::array<std::tuple<SubjectiveTime, Type>, length> values_;
};

}