//
// Created by kaede on 2019/09/26.
//

#include <GL/glew.h>
#include "DrawContext.hpp"
#include "vertex/ArrayBuffer.hpp"
#include "vertex/IndexBuffer.hpp"
#include "program/Program.hpp"
#include "Util.hpp"

namespace gl {

void DrawContext::draw() {
  Binder prg_(this->program_);
  for(auto const& it : this->buffers_) {
    Binder buf_(it.second);
    glEnableVertexAttribArray(it.first);
    checkError();
    glVertexAttribPointer(it.first, it.second->stride(), it.second->glType(), false, 0, nullptr);
    checkError();
  }
  Binder ind_(this->indicies_);

  this->indicies_->draw();
}

DrawContext::DrawContext(std::shared_ptr<Program> program)
:program_(std::move(program))
{
}

void DrawContext::attach(std::string const& attrName, std::shared_ptr<ArrayBuffer> buffer) {
  int const pos = this->program_->attribLoc(attrName);
  this->buffers_.insert_or_assign(pos, std::move(buffer));
}

void DrawContext::attach(std::shared_ptr<IndexBuffer> indicies) {
  this->indicies_ = std::move(indicies);
}

}