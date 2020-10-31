/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#pragma once

#include <string>
#include <cstddef>

namespace donut {

class Stream final {
public:
  Stream() = delete;
  Stream(Stream const&) = delete;
  Stream& operator=(Stream const&) = delete;
  static Stream open(std::string filename);
private:
  Stream(std::string&& filename, std::string&& buff)
  :filename_(std::forward<std::string>(filename))
  ,buff_(std::forward<std::string>(buff))
  ,pos_(0){
  }
private:
  std::string filename_;
  std::string buff_;
  std::size_t pos_;
public:
  [[nodiscard]] std::string const& content() const {
    return this->buff_;
  }
};

}
