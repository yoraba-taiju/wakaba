//
// Created by kaede on 2019/09/24.
//

#include "Program.hpp"
#include "../Util.hpp"

namespace gl {

std::shared_ptr<Program> Program::link(util::Logger& log, std::shared_ptr<VertexShader> const& vert, std::shared_ptr<FragmentShader> const& frag) {
  GLuint const id = glCreateProgram();
  glAttachShader(id, vert->id());
  glAttachShader(id, frag->id());
  glLinkProgram(id);

  GLint result = GL_FALSE;
  int infoLogLength = 0;
  glGetProgramiv(id, GL_LINK_STATUS, &result);
  glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
  std::string err;
  err.resize(infoLogLength);
  glGetProgramInfoLog(id, infoLogLength, nullptr, err.data());
  if(result == GL_FALSE) {
    log.fatal(err);
  }else{
    log.debug(err);
  }
  return util::make_shared<Program>(id);
}

Program::Program(GLuint id):id_(id) {

}

Program::~Program() {
  glDeleteProgram(this->id_);
}

void Program::bind() {
  glUseProgram(this->id());
}

void Program::unbind() noexcept {
  glUseProgram(0);
}

int Program::attribLoc(std::string const& name) {
  int const pos = glGetAttribLocation(this->id(), name.c_str());
  checkError();
  return pos;
}

}