#include "window.hpp"
#include "app/log.hpp"
#include <stdexcept>

namespace platform {

Window::Window() {}

Window::~Window() {
  // Window::Destroy();
}

void Window::Init(windowOpts opts) {
  window_ = glfwCreateWindow(opts.width, opts.height, opts.name, NULL, NULL);
  if (!window_) {
    TE_CRITICAL("Cannot create window");
    throw std::runtime_error("Error creating window");
  }
  TE_TRACE("Window created successfully");
}

void Window::Destroy() {
  glfwDestroyWindow(window_);
  TE_TRACE("Window destroyed successfully");
}

} // namespace platform
