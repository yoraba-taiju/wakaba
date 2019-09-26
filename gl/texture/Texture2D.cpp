//
// Created by kaede on 2019/09/23.
//

#include "Texture2D.hpp"

namespace gl {

Texture2D::~Texture2D() {
  glDeleteTextures(1, &this->id_);
}

void Texture2D::bind() {
  glBindTexture(GL_TEXTURE_2D, this->id_);
}
void Texture2D::unbind() noexcept {
  glBindTexture(GL_TEXTURE_2D, 0);
}

std::shared_ptr<Texture2D> Texture2D::create() {
  GLuint id;
  glGenTextures(1, &id);
  return util::make_shared<Texture2D>(id);
}

}