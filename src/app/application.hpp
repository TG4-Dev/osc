#pragma once
#include "gfx/vulkan-context.hpp"
#include "platform/window.hpp"

namespace core {

class Application {
public:
  Application(platform::windowOpts);
  ~Application();

  void Run();

  void Stop();

  void Update();

  platform::Window window;

  VulkanTest vulkanTest;

private:
  bool isRunning = false;
};

} // namespace core
