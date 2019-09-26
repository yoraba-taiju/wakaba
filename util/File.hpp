//
// Created by kaede on 2019/09/27.
//

#pragma once

#include <optional>
#include <string>

namespace util {

// FIXME: エラー
std::string readAllFromFile(std::string const& fname) noexcept(false);

class FileSystemError final : public std::runtime_error {
public:
  explicit FileSystemError(std::string const& msg)
  :std::runtime_error(msg) {
  }
};

}
