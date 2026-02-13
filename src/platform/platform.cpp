#include "platform.hpp"
#include "platform/log.hpp"

int platform::Init() {
  if (!glfwInit()) {
    TE_CRITICAL("Cannot initialize glfw");
    return GLFW_FALSE;
  }
  TE_TRACE("Glfw successfuly initialized");
  return GLFW_TRUE;
}

void platform::Exit() {
  glfwTerminate();
  TE_TRACE("Glfw successfuly terminated");
}
