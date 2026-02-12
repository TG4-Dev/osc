#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include <imgui_impl_vulkan.h>
#include "GLFW/glfw3.h"

class VulkanContext {
public:
  VkResult Init(GLFWwindow *window);
  VkResult Terminate();

  VkResult FrameRender(ImDrawData *draw_data);
  VkResult FramePresent();

  void ResizeSwapChain(int width, int height);

  ImGui_ImplVulkanH_Window wd;

public:
  VkDevice GetDevice() { return device_; }
  VkPhysicalDevice GetPhysicalDevice() { return physical_device_; }
  VkInstance GetInstance() { return instance_; }
  size_t GetQueueFamily() { return queue_family_; }
  VkQueue GetQueue() { return queue_; }
  VkDescriptorPool GetDescriptorPool() { return descriptor_pool_; }
  uint32_t GetMinImageCount() { return min_image_count_; }

private:
  VkResult CreateInstance();
  VkResult SelectPhysicalDevice();
  VkResult GetGraphicalQueueIndex();
  VkResult CreateLogicalDevice();
  VkResult CreateCommandPool();
  VkResult CreateDescriptorPool();

  VkResult SetupVulkanWindow(GLFWwindow *window);

private:
  VkInstance instance_ = VK_NULL_HANDLE;
  std::vector<VkPhysicalDevice> physical_devices_;
  VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
  VkDevice device_ = VK_NULL_HANDLE;
  VkSurfaceKHR surface_ = VK_NULL_HANDLE;
  size_t queue_family_;
  VkQueue queue_ = VK_NULL_HANDLE;
  VkDescriptorPool descriptor_pool_ = VK_NULL_HANDLE;

  uint32_t min_image_count_;

  bool swap_chain_rebuild_;
};
