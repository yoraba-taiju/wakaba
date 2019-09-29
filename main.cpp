#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "util/Logger.hpp"
#include "gl/texture/TextureUnit.hpp"
#include "gl/vertex/ArrayBuffer.hpp"
#include "gl/vertex/IndexBuffer.hpp"
#include "gl/program/VertexShader.hpp"
#include "gl/program/FragmentShader.hpp"
#include "gl/program/Program.hpp"
#include "gl/DrawContext.hpp"

static int _main(util::Logger& log);

int main() {
  util::Logger log;

  log.setLevel(util::Logger::DEBUG);
  try {
    return _main(log);
  } catch (std::exception& e) {
    fprintf(stderr, "Unhandled exception: \"%s\"\n", e.what());
    return -255;
  }
}

static int _main(util::Logger& log) {
  GLFWwindow *window;
  // Initialise GLFW
  if (!glfwInit()) {
    log.fatal("Failed to initialize GLFW");
    return -1;
  }
  log.debug("GLFW Initialized.");

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, 0);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1920, 1080, "YorabaTaiju", nullptr, nullptr);
  if (window == nullptr) {
    glfwTerminate();
    log.fatal("Failed to open GLFW window.");
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    log.fatal("Failed to initialize GLEW.");
    return -1;
  }
  log.debug("GLEW Initialized.");

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  std::shared_ptr<gl::ArrayBuffer> triangle = gl::ArrayBuffer::create();
  triangle->set(3, std::vector<GLfloat>{{
    +0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    +0.0f, +0.5f, 0.0f
  }});

  std::shared_ptr<gl::ArrayBuffer> color = gl::ArrayBuffer::create();
  color->set(3, std::vector<GLfloat>{{
    1,0,0,
    0,1,0,
    0,0,1,
  }});
  
  std::shared_ptr<gl::IndexBuffer> indices = gl::IndexBuffer::create();
  indices->set(GL_TRIANGLES, std::vector<uint16_t >{{
    0, 1, 2
  }});

  std::shared_ptr<gl::VertexShader> vs = gl::VertexShader::compileFromFile(log, "_resources/shaders/triangle.vs");
  std::shared_ptr<gl::FragmentShader> fs = gl::FragmentShader::compileFromFile(log, "_resources/shaders/triangle.fs");
  std::shared_ptr<gl::Program> program = gl::Program::link(log, vs, fs);

  std::shared_ptr<gl::DrawContext> ctx = gl::DrawContext::create(program);

  ctx->attach(indices);
  ctx->attach("position", triangle);
  ctx->attach("color", color);

  do {
    // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw nothing, see you in tutorial 2 !
    ctx->draw();
    
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

  } // Check if the ESC key was pressed or the window was closed
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();
  return 0;
}

