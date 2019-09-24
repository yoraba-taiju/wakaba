//
// Created by kaede on 2019/09/24.
//

#include "TextureUnit.hpp"
namespace gl {

TextureUnit::TextureUnit(util::Logger& log)
:log_(log)
,num_(0)
{
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &this->num_);
  this->activeTextures_.reserve(this->num_);
  this->log_.debug("%d Texture units.", this->num_);
}

TextureUnit::~TextureUnit() {
  this->activeTextures_.resize(0);
  this->num_ = 0;
}

}