//
// Created by kaede on 2019/09/24.
//

#include "TextureUnit.hpp"
#include "Texture2D.hpp"
#include "../../util/Logger.hpp"

namespace gl {

TextureUnit::TextureUnit()
:num_(0)
{
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &this->num_);
  this->activeTextures_.reserve(this->num_);
}

TextureUnit::~TextureUnit() {
  this->activeTextures_.resize(0);
  this->num_ = 0;
}

int TextureUnit::bind(std::shared_ptr<Texture2D> const& tex) {
  return 0;
}

std::shared_ptr<TextureUnit> TextureUnit::create() {
  return util::make_shared<TextureUnit>();
}

}