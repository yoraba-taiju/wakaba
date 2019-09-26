//
// Created by kaede on 2019/09/24.
//

#pragma once

#include <memory>
#include "../../util/Shared.hpp"
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
  void bind();
  void unbind() noexcept;
  int attribLoc(std::string const& name);
public:
  static std::shared_ptr<Program> link(util::Logger& log, std::shared_ptr<VertexShader> const& vert, std::shared_ptr<FragmentShader> const& frag);
private:
  ENABLE_SHARED_HELPER
};

}