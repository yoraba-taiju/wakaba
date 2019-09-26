//
// Created by kaede on 2019/09/25.
//

#include "ArrayBuffer.hpp"

namespace gl {

void ArrayBuffer::bind() {
  glBindBuffer(GL_ARRAY_BUFFER, this->id());
}

void ArrayBuffer::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

std::shared_ptr<ArrayBuffer> ArrayBuffer::create() {
  GLuint id;
  glGenBuffers(1, &id);
  return util::make_shared<ArrayBuffer>(id);
}

}
