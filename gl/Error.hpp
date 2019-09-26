//
// Created by kaede on 2019/09/26.
//

#pragma once

#include <string>
#include <exception>
#include <stdexcept>

namespace gl {

class Error final : public std::runtime_error {
public:
  explicit Error(std::string const &err);
};

}