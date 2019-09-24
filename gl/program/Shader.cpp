//
// Created by kaede on 2019/09/24.
//

#include <GL/glew.h>
#include <vector>

#include "Shader.hpp"

namespace gl {

GLuint Shader::compile(util::Logger& log, GLenum const target, std::string const& src) {
  GLuint id = glCreateShader(target);
  GLint result = GL_FALSE;
  int infoLogLength = 0;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
  std::string err;
  err.resize(infoLogLength);
  glGetShaderInfoLog(id, infoLogLength, nullptr, err.data());
  if(result == GL_FALSE) {
    log.fatal(err);
  }else{
    log.debug(err);
  }
  return id;
}

Shader::~Shader() {
  glDeleteShader(this->id_);
}

}