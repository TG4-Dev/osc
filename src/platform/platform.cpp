#include "platform.hpp"
#include "app/log.hpp"
#include <stdexcept>

void platform::Init() {
  if (!glfwInit()) {
    TE_CRITICAL("Cannot initialize glfw");
    throw std::runtime_error("glfw init error");
  }
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  TE_TRACE("Glfw successfuly initialized");
}

void platform::Exit() {
  glfwTerminate();
  TE_TRACE("Glfw successfuly terminated");
}
