#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "GLFW/glfw3.h"

class VulkanContext {
public:
  VkResult Init(GLFWwindow *window);
  VkResult Terminate();

private:
  VkResult CreateInstance();
  VkResult EnumeratePhysicalDevices();
  VkResult SelectPhysicalDevice();
  VkResult CreateLogicalDevice();
  VkResult CreateSurface(GLFWwindow *window);

  int RateDeviceSuitability(VkPhysicalDevice device);
  uint32_t SelectQueueFamilyIndex(VkPhysicalDevice physical_device);

private:
  VkInstance instance_ = VK_NULL_HANDLE;
  std::vector<VkPhysicalDevice> physical_devices_;
  VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
  VkDevice device_ = VK_NULL_HANDLE;
  VkSurfaceKHR surface_ = VK_NULL_HANDLE;
	VkQueue graphics_queue_;
	VkQueue present_quueu_;
};
