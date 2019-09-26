//
// Created by kaede on 2019/09/26.
//

#include <string>
#include <GL/glew.h>
#include "Util.hpp"
#include "Error.hpp"

namespace gl {

void checkError() noexcept(false) {
  GLenum const err = glGetError();
  if (err == GL_NO_ERROR) {
    return;
  }
  std::string msg = reinterpret_cast<const char *>(gluErrorString(err));
  throw Error("[OpenGL Error: \""+std::move(msg)+"\"]");
}

template<>
GLenum enumOf<GLfloat>() {
  return GL_FLOAT;
}

template<>
GLenum enumOf<GLushort>() {
  return GL_UNSIGNED_SHORT;
}

template<>
GLenum enumOf<GLuint>() {
  return GL_UNSIGNED_INT;
}

}