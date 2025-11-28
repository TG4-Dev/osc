#include "vulkan-context.hpp"
#include "GLFW/glfw3.h"
#include "platform/log.hpp"
#include <cstddef>
#include <map>
#include <cstdint>
#include <vulkan/vulkan_core.h>

VkResult VulkanContext::Init() {
  VkResult result = CreateInstance();
  result = EnumeratePhysicalDevices();
  // result = SelectPhysicalDevice();

  return result;
}

VkResult VulkanContext::CreateInstance() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount = 0;

  std::vector<const char *> requiredExtensions;

#ifdef __APPLE__
  for (uint32_t i = 0; i < glfwExtensionCount; i++) {
    requiredExtensions.emplace_back(glfwExtensions[i]);
  }

  requiredExtensions.emplace_back(
      VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

  createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

  createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
  createInfo.ppEnabledExtensionNames = requiredExtensions.data();
#endif

  VkResult result = vkCreateInstance(&createInfo, nullptr, &instance_);
  if (result != VK_SUCCESS) {
    TE_CRITICAL("Cannot create Vulkan instance");
    return result;
  }
  TE_TRACE("Vulkan instance successfully created");
  return VK_SUCCESS;
}

VkResult VulkanContext::EnumeratePhysicalDevices() {
  uint32_t physical_device_count = 0;

  VkResult result = VK_SUCCESS;

  // Getting amount of phys devices
  result =
      vkEnumeratePhysicalDevices(instance_, &physical_device_count, nullptr);

  if (result == VK_SUCCESS) {
    physical_devices_.resize(physical_device_count);
    vkEnumeratePhysicalDevices(instance_, &physical_device_count,
                               &physical_devices_[0]);

    TE_TRACE("Found {} VkDevices", physical_device_count);
  }
  return result;
}

VkResult VulkanContext::SelectPhysicalDevice() {
  std::multimap<int, VkPhysicalDevice> candidates;

  for( const auto& device : physical_devices_) {
    int score = RateDeviceSuitability(device);
    candidates.insert(std::make_pair(score, device));
  }

  if (candidates.rbegin()->first > 0) {
   physical_device_ = candidates.rbegin()->second; 
   return VK_SUCCESS;
  } else {
    return VK_ERROR_INITIALIZATION_FAILED;
  }
}


int VulkanContext::RateDeviceSuitability(VkPhysicalDevice device) {

  VkPhysicalDeviceProperties device_properties{};
  VkPhysicalDeviceFeatures device_features{};


  vkGetPhysicalDeviceProperties(device, &device_properties);
  vkGetPhysicalDeviceFeatures(device, &device_features);
  int score = 0;
  if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }

  score += device_properties.limits.maxImageDimension2D;

  if (!device_features.geometryShader) {
    return 0;
  }

  return score;
}
VkResult VulkanContext::Terminate() {
  if (instance_ == VK_NULL_HANDLE) {
    TE_WARN("Attemted to terminate null Vulkan instance");
    return VK_ERROR_INITIALIZATION_FAILED;
  }

  vkDestroyInstance(instance_, nullptr);
  instance_ = VK_NULL_HANDLE;
  TE_TRACE("Vulkan successfully terminated");
  return VK_SUCCESS;
}
