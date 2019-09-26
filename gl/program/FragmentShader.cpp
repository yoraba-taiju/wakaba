//
// Created by kaede on 2019/09/24.
//

#include <GL/glew.h>

#include "FragmentShader.hpp"

namespace gl {

FragmentShader::FragmentShader(GLuint const id): Shader(id) {

}

std::shared_ptr<FragmentShader> FragmentShader::compile(util::Logger &log, std::string const &src) {
  GLuint const id = Shader::compile(log, GL_FRAGMENT_SHADER, src);
  return util::make_shared<FragmentShader>(id);
}

std::shared_ptr<FragmentShader> FragmentShader::compileFromFile(util::Logger &log, std::string const& fileName) {
  GLuint const id = Shader::compileFromFile(log, GL_FRAGMENT_SHADER, fileName);
  return util::make_shared<FragmentShader>(id);
}

}