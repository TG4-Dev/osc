#include "application.hpp"
#include "platform/platform.hpp"

namespace core {

Application::Application(platform::windowOpts opts) {
  platform::Init();

  vulkan_ctx_.CreateInstance();

  platform_window_.Init(opts);
}

Application::~Application() {
  platform_window_.Destroy();
  platform::Exit();
  vulkan_ctx_.Terminate();
}

void Application::Run() {
  is_running_ = true;

  while (is_running_) {
    glfwPollEvents();

    if (glfwWindowShouldClose(platform_window_.GetWindowHandle()))
      Stop();
  }
}

void Application::Stop() { is_running_ = false; }
} // namespace core
