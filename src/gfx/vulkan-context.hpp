#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

class VulkanContext {
public:
  VkResult Init();
  VkResult CreateInstance();
  VkResult Terminate();
  VkResult EnumeratePhysicalDevices();
  VkResult SelectPhysicalDevice();

private:
  VkInstance instance_;
  std::vector<VkPhysicalDevice> physical_devices_;
};
