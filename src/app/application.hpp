#pragma once
#include "gfx/buffer.hpp"
#include "gfx/vertex.hpp"
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

private:
  void LoadResources();
  platform::Window platform_window_;
  VulkanContext vulkan_ctx_;
  bool is_running_ = false;

  gfx::Buffer vertex_buffer_;
};

} // namespace core
