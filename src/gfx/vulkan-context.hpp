#pragma once
#include <vulkan/vulkan.h>

class VulkanContext {
public:
  void Terminate();

  void CreateInstance();

private:
  VkInstance instance;
};
