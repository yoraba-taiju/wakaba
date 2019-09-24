//
// Created by kaede on 2019/09/23.
//

#include "Texture.hpp"

namespace gl {

Texture::~Texture() {
  this->pool_->release(this->id_);
  this->id_ = 0;
}

}