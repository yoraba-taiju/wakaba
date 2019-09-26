//
// Created by kaede on 2019/09/24.
//

#include <GL/glew.h>
#include <vector>

#include "Shader.hpp"
#include "../../util/File.hpp"

namespace gl {


Shader::~Shader() {
  glDeleteShader(this->id_);
}

GLuint Shader::compile(util::Logger& log, GLenum const target, std::string const& src) {
  GLuint const id = glCreateShader(target);
  auto cstr = src.c_str();
  glShaderSource(id, 1, &cstr, nullptr);
  glCompileShader(id);
  
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
    if(!err.empty()) {
      log.debug(err);
    }
  }
  return id;
}

GLuint Shader::compileFromFile(util::Logger &log, GLenum target, std::string const& fileName) {
  std::string src = util::readAllFromFile(fileName);
  return Shader::compile(log, target, src);
}

}