/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#pragma once

#include <algorithm>

template<typename T> class Optional final {
public:
  explicit Optional(T& v):value_(&v){}
  Optional():value_(nullptr){}
  Optional(Optional const&) = delete;
  Optional(Optional&&) = delete;
  Optional& operator=(Optional const&) = delete;
  Optional& operator=(Optional&&) = delete;
  [[ nodiscard ]] explicit operator bool() const{
    return this->has_value();
  }
  [[nodiscard]] bool has_value() const {
    return this->value_ != nullptr;
  }
  T& value() {
    if(this->value_ == nullptr) {
      throw std::bad_optional_access();
    }
    return *this->value_;
  }
private:
  T* value_;
};
