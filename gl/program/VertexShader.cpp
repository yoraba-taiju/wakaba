//
// Created by kaede on 2019/09/24.
//

#include "VertexShader.hpp"
#include "../../util/Shared.hpp"
#include <GL/glew.h>

namespace gl {
VertexShader::VertexShader(GLuint const id): Shader(id) {

}

std::shared_ptr<VertexShader> VertexShader::compile(util::Logger &log, std::string const& src) {
  GLuint const id = Shader::compile(log, GL_VERTEX_SHADER, src);
  return util::make_shared<VertexShader>(id);
}

std::shared_ptr<VertexShader> VertexShader::compileFromFile(util::Logger &log, std::string const& fileName) {
  GLuint const id = Shader::compileFromFile(log, GL_VERTEX_SHADER, fileName);
  return util::make_shared<VertexShader>(id);
}

}