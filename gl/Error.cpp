//
// Created by kaede on 2019/09/26.
//

#include "Error.hpp"

namespace gl {

Error::Error(std::string err)
:msg_(std::move(err)){

}

const char* Error::what() const noexcept {
  return this->msg_.c_str();
}

Error::~Error() noexcept {

}

}
