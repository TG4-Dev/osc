#include "application.hpp"
#include "platform/platform.hpp"
#include <vulkan/vk_enum_string_helper.h>
#include <stdexcept>

namespace core {

Application::Application(platform::windowOpts opts) {
  if(platform::Init() == GLFW_FALSE) {
    throw std::runtime_error("glfw init error");
	}
	if(platform_window_.Init(opts) == GLFW_FALSE) {
		throw std::runtime_error("Error creating window");
	}
  if(VkResult res = vulkan_ctx_.CreateInstance(); res != VK_SUCCESS) {
		throw std::runtime_error(string_VkResult(res));
	}
}

Application::~Application() {
	vulkan_ctx_.Terminate(); 
	platform_window_.Destroy();
  platform::Exit();
}

void Application::Run() {
  is_running_ = true;

  while (is_running_) {
    glfwPollEvents();

    if (glfwWindowShouldClose(platform_window_.GetWindowHandle()))
      Stop();
  }
}

void Application::Stop() { 
	is_running_ = false; 
}

} // namespace core
