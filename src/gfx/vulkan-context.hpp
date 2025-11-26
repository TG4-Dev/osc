#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

class VulkanContext {
public:
  VkResult CreateInstance();
  VkResult Terminate();

private:
  VkInstance instance_;
};
