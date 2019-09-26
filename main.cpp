#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "util/Logger.hpp"
#include "gl/texture/TextureUnit.hpp"
#include "gl/vertex/ArrayBuffer.hpp"
#include "gl/vertex/IndexBuffer.hpp"

static int _main(util::Logger& log);

int main() {
  util::Logger log;

  log.setLevel(util::Logger::DEBUG);
  try {
    return _main(log);
  } catch (std::exception& e) {
    fputs(e.what(), stderr);
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
  triangle->set(std::vector<GLfloat>{{
    +0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    +0.0f, +0.5f, 0.0f
  }});
  
  std::shared_ptr<gl::IndexBuffer> indices = gl::IndexBuffer::create();
  indices->set(std::vector<uint16_t >{{
    0, 1, 2
  }});
  

  do {
    // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw nothing, see you in tutorial 2 !
    indices->draw();
    
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

