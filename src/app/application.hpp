#pragma once
#include "gfx/vulkan-context.hpp"
#include "platform/window.hpp"
#include "ui/imgui-context.hpp"

namespace core {

class Application {
public:
  Application(platform::windowOpts);
  ~Application();

  void Run();

  void Update();

private:
  platform::Window platform_window_;
  VulkanContext vulkan_ctx_;
  ImGuiContext imgui_ctx_;
};

} // namespace core
