#include "window.hpp"
#include <stdexcept>

namespace platform {

Window::Window() {}

Window::~Window() { Window::Destroy(); }

void Window::Init(windowOpts opts) {
  window = glfwCreateWindow(opts.width, opts.height, opts.name, NULL, NULL);
  if (!window) {
    throw std::runtime_error("Error creating window");
  }
}

void Window::Destroy() { glfwDestroyWindow(window); }

} // namespace platform
