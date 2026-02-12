#pragma once
#include "gfx/vulkan-context.hpp"
#include "platform/window.hpp"

class ImGuiContext {
public:
  void Run(VulkanContext *v, platform::Window *window);
  void Terminate();
};
