//
// Created by kaede on 2019/09/26.
//

#include "IndexBuffer.hpp"

namespace gl {

void IndexBuffer::bind() {
  glBindBuffer(GL_ARRAY_BUFFER, this->id());
}

void IndexBuffer::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

std::shared_ptr<IndexBuffer> IndexBuffer::create() {
  GLuint id;
  glGenBuffers(1, &id);
  checkError();
  return util::make_shared<IndexBuffer>(id);

}

}
