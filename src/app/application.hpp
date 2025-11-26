#pragma once
#include "gfx/vulkan-context.hpp"
#include "platform/window.hpp"
#include <vulkan/vulkan_core.h>

namespace core {

class Application {
public:
  Application(platform::windowOpts);
  ~Application();

  void Run();

  void Stop();

  void Update();

private:
  platform::Window platform_window_;

  VulkanContext vulkan_ctx_;

  bool is_running_ = false;
};

} // namespace core
