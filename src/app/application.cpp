#include "application.hpp"
#include "platform/log.hpp"
#include "platform/platform.hpp"
#include <stdexcept>
#include <vulkan/vk_enum_string_helper.h>

#include "GLFW/glfw3.h"

namespace core {

Application::Application(platform::windowOpts opts) {
  TE_TRACE(glfwGetVersionString());
  if (platform::Init() == GLFW_FALSE) {
    throw std::runtime_error("glfw init error");
  }
  if (platform_window_.Init(opts) == GLFW_FALSE) {
    throw std::runtime_error("Error creating window");
  }
  if (VkResult res = vulkan_ctx_.Init(platform_window_.GetWindowHandle());
      res != VK_SUCCESS) {
    throw std::runtime_error(string_VkResult(res));
  }
}

Application::~Application() {
  imgui_ctx_.Terminate();
  vulkan_ctx_.Terminate();
  platform_window_.Destroy();
  platform::Exit();
}

void Application::Run() {
  imgui_ctx_.Run(&vulkan_ctx_, &platform_window_);
}

} // namespace core
