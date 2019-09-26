//
// Created by kaede on 2019/09/26.
//

#pragma once

#include <string>
#include <exception>

namespace gl {

class Error final : public std::exception {
private:
  std::string msg_;
public:
  explicit Error(std::string err);
  virtual ~Error() noexcept override;
  virtual const char* what() const noexcept override;

};

}