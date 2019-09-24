//
// Created by kaede on 2019/09/24.
//

#pragma once

#include <memory>
#include "../../util/SharedUtil.hpp"
#include "VertexShader.hpp"
#include "FragmentShader.hpp"

namespace gl {

class Program final{
private:
  GLuint const id_;
  explicit Program(GLuint id);
public:
  ~Program();
  [[nodiscard]] GLuint id() const {
    return this->id_;
  }
public:
  static std::shared_ptr<Program> link(util::Logger& log, std::shared_ptr<VertexShader> vert, std::shared_ptr<FragmentShader> frag);
private:
  ENABLE_SHARED_HELPER
};

}