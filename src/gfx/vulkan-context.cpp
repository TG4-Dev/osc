#include "vulkan-context.hpp"
#include "GLFW/glfw3.h"
#include "platform/log.hpp"
#include <vulkan/vulkan_core.h>

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

  VkResult result = vkCreateInstance(&createInfo, nullptr, &instance_);
  if (result != VK_SUCCESS) {
    TE_CRITICAL("Cannot create Vulkan instance");
    return result;
  }
  TE_TRACE("Vulkan instance successfully created");
  return VK_SUCCESS;
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
