//
// Created by kaede on 2019/09/26.
//

#include "IndexBuffer.hpp"

namespace gl {

std::shared_ptr<IndexBuffer> IndexBuffer::create() {
  GLuint id;
  glGenBuffers(1, &id);
  checkError();
  return util::make_shared<IndexBuffer>(id);

}

void IndexBuffer::draw() {
  glDrawElements(GL_TRIANGLES, this->size(), GL_UNSIGNED_SHORT, nullptr);
  checkError();
}

}
