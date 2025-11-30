#include "vulkan-context.hpp"
#include "platform/log.hpp"
#include <cstdint>
#include <optional>
#include <set>
#include <map>
#include <vulkan/vulkan_core.h>

struct VulkanContext::QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

VkResult VulkanContext::Init(GLFWwindow *window) {
  VkResult result = CreateInstance();
	if (result != VK_SUCCESS) {
		return result;
	}	
	result = CreateSurface(window);
	if (result != VK_SUCCESS) {
		return result;
	}
  result = EnumeratePhysicalDevices();
	if (result != VK_SUCCESS) {
		return result;
	}
  result = SelectPhysicalDevice();
	if (result != VK_SUCCESS) {
		return result;
	} 
  result = CreateLogicalDevice();
	if (result != VK_SUCCESS) {
		return result;
	}


  return result;
}

VkResult VulkanContext::CreateInstance() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "osc";
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

  createInfo.enabledLayerCount = 0;

  std::vector<const char *> requiredExtensions;

  for (uint32_t i = 0; i < glfwExtensionCount; i++) {
    requiredExtensions.emplace_back(glfwExtensions[i]);
  }

#ifdef __APPLE__
  requiredExtensions.emplace_back(
      VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

  createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

  createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
  createInfo.ppEnabledExtensionNames = requiredExtensions.data();

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

  VkPhysicalDeviceProperties device_properties{};
  for (const auto &device : physical_devices_) {
    vkGetPhysicalDeviceProperties(device, &device_properties);
    TE_TRACE("Found physical device: {}", device_properties.deviceName);
    int score = RateDeviceSuitability(device);
    candidates.insert(std::make_pair(score, device));
  }

  if (candidates.rbegin()->first > 0) {
    VkPhysicalDeviceProperties device_properties;

    physical_device_ = candidates.rbegin()->second;

    vkGetPhysicalDeviceProperties(physical_device_, &device_properties);

    TE_TRACE("Selected physical device: {} ", device_properties.deviceName);
    return VK_SUCCESS;
  } else {
    return VK_ERROR_INITIALIZATION_FAILED;
  }
}

VkResult VulkanContext::CreateLogicalDevice() {
	QueueFamilyIndices indices = FindFamilyIndices(physical_device_);
	std::set<uint32_t> uniqueQueueFamilies = {
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};

	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queue_create_infos.push_back(queueCreateInfo);
	}

  // Filling logical device creation info
  VkDeviceCreateInfo device_create_info{};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
  device_create_info.pQueueCreateInfos = queue_create_infos.data();

  // Extensions
  const std::vector<const char *> device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  device_create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
  device_create_info.ppEnabledExtensionNames = device_extensions.data();

  // Creating logical device
  VkResult result = vkCreateDevice(physical_device_, &device_create_info, nullptr, &device_);

	vkGetDeviceQueue(device_, indices.graphicsFamily.value(), 0, &graphics_queue_);
	vkGetDeviceQueue(device_, indices.presentFamily.value(), 0, &present_queue_);

  TE_TRACE("Device Created successfully");
  return result;
}

VkResult VulkanContext::CreateSurface(GLFWwindow *window) {
  if (!window) {
    TE_ERROR("Cannot create surface");
    return VK_ERROR_INITIALIZATION_FAILED;
  }
  VkResult result =
      glfwCreateWindowSurface(instance_, window, nullptr, &surface_);

  TE_TRACE("Surface created successfully");
  return result;
}

int VulkanContext::RateDeviceSuitability(VkPhysicalDevice physical_device) {
  int score = 0;

	QueueFamilyIndices indices = FindFamilyIndices(physical_device);
	if (!indices.isComplete()) {
		return score;
	}

  VkPhysicalDeviceProperties device_properties{};
  VkPhysicalDeviceFeatures device_features{};

  vkGetPhysicalDeviceProperties(physical_device, &device_properties);
  vkGetPhysicalDeviceFeatures(physical_device, &device_features);
  if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }

  score += device_properties.limits.maxImageDimension2D;

  /*if (!device_features.geometryShader) {*/
  /*  return 0;*/
  /*}*/

  return score;
}

VulkanContext::QueueFamilyIndices VulkanContext::FindFamilyIndices(VkPhysicalDevice device) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_, &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

VkResult VulkanContext::Terminate() {
	if (surface_ == VK_NULL_HANDLE) {
    TE_WARN("Attemted to terminate null Vulkan surface");
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	vkDestroySurfaceKHR(instance_, surface_, nullptr);

  if (instance_ == VK_NULL_HANDLE) {
    TE_WARN("Attemted to terminate null Vulkan instance");
    return VK_ERROR_INITIALIZATION_FAILED;
  }

  vkDestroyInstance(instance_, nullptr);
  instance_ = VK_NULL_HANDLE;

  TE_TRACE("Vulkan successfully terminated");
  return VK_SUCCESS;
}
