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
	VkResult CreateSwapchain(GLFWwindow *window);
	VkResult CreateImageViews();

  int RateDeviceSuitability(VkPhysicalDevice device);

	struct QueueFamilyIndices;
	QueueFamilyIndices FindFamilyIndices(VkPhysicalDevice);

	struct SwapChainSupportDetails;
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice);

private:
  VkInstance instance_ = VK_NULL_HANDLE;
  std::vector<VkPhysicalDevice> physical_devices_;
  VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
  VkDevice device_ = VK_NULL_HANDLE;
  VkSurfaceKHR surface_ = VK_NULL_HANDLE;
	VkQueue graphics_queue_;
	VkQueue present_queue_;
	VkSwapchainKHR swapchain_ = VK_NULL_HANDLE;
	std::vector<VkImage> swapchain_images_;
	VkFormat swapchain_image_format_;
	VkExtent2D swapchain_extent_;
	std::vector<VkImageView> swapchain_image_views_;

};
