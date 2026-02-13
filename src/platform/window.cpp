#include "window.hpp"
#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "platform/log.hpp"

namespace platform {

Window::Window() {}

Window::~Window() {}

int Window::Init(windowOpts opts) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

  window_ =
      glfwCreateWindow(static_cast<int>(opts.width * main_scale),
                       static_cast<int>(opts.height * main_scale), opts.name, nullptr, nullptr);
  if (!window_) {
    TE_CRITICAL("Cannot create window");
    return GLFW_FALSE;
  }
  TE_TRACE("Window created successfully");
  return GLFW_TRUE;
}

void Window::Destroy() {
  glfwDestroyWindow(window_);
  TE_TRACE("Window destroyed successfully");
}

} // namespace platform
