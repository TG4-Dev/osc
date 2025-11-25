#include "vulkan-context.hpp"
#include "GLFW/glfw3.h"
#include "platform/log.hpp"
#include <stdexcept>

void VulkanContext::CreateInstance() {
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

  if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS) {
    TE_CRITICAL("Cannot create Vulkan instance");
    throw std::runtime_error("failed to create instance!");
  }
  TE_TRACE("Vulkan instance successfully created");
}

void VulkanContext::Terminate() {
  vkDestroyInstance(instance_, nullptr);
  TE_TRACE("Vulkan successfully terminated");
}
