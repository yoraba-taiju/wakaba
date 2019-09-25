//
// Created by kaede on 2019/09/25.
//

#include "Buffer.hpp"

gl::Buffer::Buffer(GLuint id):id_(id) {

}

gl::Buffer::~Buffer() {
  glDeleteBuffers(this->id_);
}

