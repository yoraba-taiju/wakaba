//
// Created by kaede on 2019/09/23.
//

#include <GL/glew.h>
#include "TexturePool.hpp"

namespace gl {

std::shared_ptr<Texture> TexturePool::alloc() {
  if (this->textures_.empty()) {
    GLuint id = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    return std::make_shared<Texture>(this->shared_from_this(), id);
  }
  GLuint handle = this->textures_.back();
  this->textures_.pop_back();
  return std::make_shared<Texture>(this->shared_from_this(), handle);
}

void TexturePool::release(GLuint const handle) {
  this->textures_.emplace_back(handle);
}

TexturePool::TexturePool(util::Logger& log)
:log_(log)
{
}

TexturePool::~TexturePool() {
  glDeleteTextures(this->textures_.size(), this->textures_.data());
  this->textures_.resize(0);
}

}