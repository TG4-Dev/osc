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

  LoadResources();
}

Application::~Application() {
  vertex_buffer_.Destroy();
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

    // DrawFrame()
  }
}

void Application::Stop() { is_running_ = false; }

void Application::LoadResources() {
  // 1. Определяем геометрию (треугольник)
  std::vector<gfx::Vertex> vertices = {
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // Top, Red
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},  // Bottom Right, Green
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}  // Bottom Left, Blue
  };

  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

  // 2. Создаем буфер через нашу обертку
  // Используем HOST_VISIBLE | HOST_COHERENT для прямой записи с CPU
  vertex_buffer_.Create(vulkan_ctx_, bufferSize,
                        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  // 3. Загружаем данные в GPU память
  vertex_buffer_.UploadData(vertices.data(), bufferSize);

  TE_TRACE("Vertex buffer created successfully");
}

} // namespace core
