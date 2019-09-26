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
  std::string msg = reinterpret_cast<const char *>(glewGetErrorString(err));
  throw Error(std::move(msg));
}

}