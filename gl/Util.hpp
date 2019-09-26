//
// Created by kaede on 2019/09/26.
//

#pragma once

#include "Error.hpp"

namespace gl {

void checkError() noexcept(false);

template <typename T>
struct Binder final {
private:
  T obj_;
public:
  explicit Binder(T obj): obj_(obj) {
    obj_->bind();
  }
  ~Binder() noexcept {
    obj_->unbind();
  }
public: // deleted methods
  Binder() = delete;
  Binder(Binder const&) = delete;
  Binder& operator=(Binder) = delete;
  Binder& operator=(Binder const&) = delete;
};

}
