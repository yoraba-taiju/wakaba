//
// Created by kaede on 2019/09/25.
//

#include "ArrayBuffer.hpp"

namespace gl {

std::shared_ptr<ArrayBuffer> ArrayBuffer::create() {
  GLuint id;
  glGenBuffers(1, &id);
  return util::make_shared<ArrayBuffer>(id);
}

}
