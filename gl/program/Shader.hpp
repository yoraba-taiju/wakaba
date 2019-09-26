//
// Created by kaede on 2019/09/24.
//

#pragma once

#include "../../util/Logger.hpp"
#include <GL/glew.h>

namespace gl {

class Shader {
protected:
  GLuint const id_;
protected:
  explicit Shader(GLuint const id):id_(id) {
  }

  static GLuint compile(util::Logger &log, GLenum target, std::string const &src);
  static GLuint compileFromFile(util::Logger &log, GLenum target, std::string const& fileName);

  ~Shader();
public:
  Shader() = delete;
public:
  [[nodiscard]] GLuint id() const {
    return this->id_;
  }
};

}
