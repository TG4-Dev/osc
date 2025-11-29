#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

class VulkanContext {
public:
  VkResult Init();
  VkResult Terminate();

private:
  VkResult CreateInstance();
  VkResult EnumeratePhysicalDevices();
  VkResult SelectPhysicalDevice();
  VkResult CreateLogicalDevice();
  
  int RateDeviceSuitability(VkPhysicalDevice device);
  uint32_t SelectQueueFamilyIndex(VkPhysicalDevice device);

private:
  VkInstance instance_ = VK_NULL_HANDLE;
  std::vector<VkPhysicalDevice> physical_devices_;
  VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
  uint32_t queue_family_index_ = 0;
  VkDevice device_ = VK_NULL_HANDLE;
};
