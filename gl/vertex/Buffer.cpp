//
// Created by kaede on 2019/09/25.
//

#include <memory>
#include "Buffer.hpp"

namespace gl {

Buffer::Buffer(GLuint const id)
    : id_(id) {

}

Buffer::~Buffer() {
  glDeleteBuffers(1, &this->id_);
}

}