#include "vulkan-context.hpp"
#include "platform/log.hpp"
#include <cstddef>
#include <cstdint>
#include <vulkan/vulkan_core.h>

VkResult VulkanContext::Init(GLFWwindow *window) {
  swap_chain_rebuild_ = false;

  VkResult result = CreateInstance();
  if (result != VK_SUCCESS) {
    return result;
  }
  result = SelectPhysicalDevice();
  if (result != VK_SUCCESS) {
    return result;
  }
  result = GetGraphicalQueueIndex();
  if (result != VK_SUCCESS) {
    return result;
  }
  result = CreateLogicalDevice();
  if (result != VK_SUCCESS) {
    return result;
  }
  result = CreateDescriptorPool();
  if (result != VK_SUCCESS) {
    return result;
  }
  result = SetupVulkanWindow(window);
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

VkResult VulkanContext::SelectPhysicalDevice() {
  uint32_t physicalDeviceCount;

  VkResult res = vkEnumeratePhysicalDevices(instance_, &physicalDeviceCount,
                                            &physical_devices_[0]);
  if (res != VK_SUCCESS) {
    TE_CRITICAL("Cannot enumerate physical devices");
    return res;
  }

  physical_devices_.resize(physicalDeviceCount);
  res = vkEnumeratePhysicalDevices(instance_, &physicalDeviceCount,
                                   &physical_devices_[0]);
  TE_TRACE("Found {} VkDevices", physicalDeviceCount);

  for (size_t i = 0; i < physicalDeviceCount; ++i) {
    physical_device_ = physical_devices_[i];
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physical_device_, &deviceProperties);
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      break;
    }
  }
  TE_TRACE("Physical device successfully selected");
  return VK_SUCCESS;
}

VkResult VulkanContext::GetGraphicalQueueIndex() {
  uint32_t familyCount;

  vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &familyCount,
                                           nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilyProperties(familyCount);

  vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &familyCount,
                                           queueFamilyProperties.data());

  for (size_t i = 0; i < familyCount; i++) {
    if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
      queue_family_ = i;
      TE_TRACE("Graphical queue index successfully gained");
      return VK_SUCCESS;
    }
  }

  TE_ERROR("Graphical queue index wasn't gained");
  return VK_ERROR_INITIALIZATION_FAILED;
}

VkResult VulkanContext::CreateLogicalDevice() {
  std::vector<const char *> device_extensions;
  device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

  float queue_priority = 0.0f;
  VkDeviceQueueCreateInfo queueCreateInfo{};
  queueCreateInfo.flags = VkDeviceQueueCreateFlags();
  queueCreateInfo.queueFamilyIndex = static_cast<uint32_t>(queue_family_);
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = &queue_priority;

  VkDeviceCreateInfo deviceCreateInfo{};
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.queueCreateInfoCount = 1;
  deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
  deviceCreateInfo.enabledExtensionCount = device_extensions.size();
  deviceCreateInfo.ppEnabledExtensionNames = device_extensions.data();

  VkResult res =
      vkCreateDevice(physical_device_, &deviceCreateInfo, nullptr, &device_);
  if (!device_) {
    TE_ERROR("Error creating logical device");
    return VK_ERROR_INITIALIZATION_FAILED;
  }
  TE_TRACE("Device created successfully");

  vkGetDeviceQueue(device_, queue_family_, 0, &queue_);
  if (!queue_) {
    TE_ERROR("Error getting queue");
    return VK_ERROR_INITIALIZATION_FAILED;
  }
  TE_TRACE("Queue gained successfully");

  return res;
}

VkResult VulkanContext::CreateDescriptorPool() {
  std::vector<VkDescriptorPoolSize> pool_sizes = {
      {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
       IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE}};

  VkDescriptorPoolCreateInfo poolCreateInfo{};
  poolCreateInfo.maxSets = 1000;
  poolCreateInfo.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
  poolCreateInfo.pPoolSizes = pool_sizes.data();

  VkResult res = vkCreateDescriptorPool(device_, &poolCreateInfo, nullptr,
                                        &descriptor_pool_);
  if (res != VK_SUCCESS) {
    TE_ERROR("Error creating descriptor pool");
    return res;
  }
  TE_TRACE("Descriptor pool created successfully");
  return res;
}

VkResult VulkanContext::SetupVulkanWindow(GLFWwindow *window) {
  int w;
  int h;

  glfwGetFramebufferSize(window, &w, &h);

  min_image_count_ = 2;

  const VkFormat requestSurfaceImageFormat[] = {
      VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM,
      VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};

  const VkColorSpaceKHR requestSurfaceColorSpace =
      VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

  glfwCreateWindowSurface(instance_, window, nullptr, &surface_);
  if (!surface_) {
    TE_ERROR("Error creating window surface");
    return VK_ERROR_INITIALIZATION_FAILED;
  }

  wd.Surface = surface_;
  wd.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(
      physical_device_, wd.Surface, requestSurfaceImageFormat,
      (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat),
      requestSurfaceColorSpace);

#ifdef APP_USE_UNLIMITED_FRAME_RATE
  VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR,
                                      VK_PRESENT_MODE_IMMEDIATE_KHR,
                                      VK_PRESENT_MODE_FIFO_KHR};
#else
  VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
#endif

  wd.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(
      physical_device_, wd.Surface, &present_modes[0],
      IM_ARRAYSIZE(present_modes));

  ImGui_ImplVulkanH_CreateOrResizeWindow(instance_, physical_device_, device_,
                                         &wd, queue_family_, nullptr, w, h,
                                         min_image_count_, 0);
  TE_TRACE("Vulkan window setup successfully done");
  return VK_SUCCESS;
}

VkResult VulkanContext::FrameRender(ImDrawData *draw_data) {
  VkSemaphore imageAcquiredSemaphore =
      wd.FrameSemaphores[wd.SemaphoreIndex].ImageAcquiredSemaphore;
  VkSemaphore renderCompleteSemaphore =
      wd.FrameSemaphores[wd.SemaphoreIndex].RenderCompleteSemaphore;

  VkResult res =
      vkAcquireNextImageKHR(device_, wd.Swapchain, UINT64_MAX,
                            imageAcquiredSemaphore, nullptr, &wd.FrameIndex);
  if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) {
    swap_chain_rebuild_ = true;
  }
  if (res != VK_SUCCESS) {
    TE_ERROR("Error getting next image in FrameRender");
    return res;
  }

  ImGui_ImplVulkanH_Frame fd = wd.Frames[wd.FrameIndex];
  {
    res = vkWaitForFences(device_, 1, &fd.Fence, VK_TRUE, UINT64_MAX);
    if (res != VK_SUCCESS) {
      TE_ERROR("Error waiting for fences");
      return res;
    }

    res = vkResetFences(device_, 1, &fd.Fence);
    if (res != VK_SUCCESS) {
      TE_ERROR("Error reseting fences");
      return res;
    }
  }

  {
    res = vkResetCommandPool(device_, fd.CommandPool, 0);
    if (res != VK_SUCCESS) {
      TE_ERROR("Error reseting command pool");
      return res;
    }

    VkCommandBufferBeginInfo info{};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    res = vkBeginCommandBuffer(fd.CommandBuffer, &info);
    if (res != VK_SUCCESS) {
      TE_ERROR("Error begining command buffer");
      return res;
    }
  }
  {
    VkRenderPassBeginInfo info{};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = wd.RenderPass;
    info.framebuffer = fd.Framebuffer;
    info.renderArea.extent.width = wd.Width;
    info.renderArea.extent.height = wd.Height;
    info.clearValueCount = 1;
    info.pClearValues = &wd.ClearValue;
    vkCmdBeginRenderPass(fd.CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
  }

  ImGui_ImplVulkan_RenderDrawData(draw_data, fd.CommandBuffer);

  vkCmdEndRenderPass(fd.CommandBuffer);
  {
    VkPipelineStageFlags wait_stage =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &imageAcquiredSemaphore;
    info.pWaitDstStageMask = &wait_stage;
    info.commandBufferCount = 1;
    info.pCommandBuffers = &fd.CommandBuffer;
    info.signalSemaphoreCount = 1;
    info.pSignalSemaphores = &renderCompleteSemaphore;

    res = vkEndCommandBuffer(fd.CommandBuffer);
    if (res != VK_SUCCESS) {
      TE_ERROR("Error ending command buffer");
      return res;
    }
    res = vkQueueSubmit(queue_, 1, &info, fd.Fence);
    if (res != VK_SUCCESS) {
      TE_ERROR("Error submiting queue");
      return res;
    }
  }

  return VK_SUCCESS;
}
VkResult VulkanContext::FramePresent() {
  if (swap_chain_rebuild_) {
    return VK_SUCCESS;
  }
  VkSemaphore render_complete_semaphore =
      wd.FrameSemaphores[wd.SemaphoreIndex].RenderCompleteSemaphore;
  VkPresentInfoKHR info = {};
  info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  info.waitSemaphoreCount = 1;
  info.pWaitSemaphores = &render_complete_semaphore;
  info.swapchainCount = 1;
  info.pSwapchains = &wd.Swapchain;
  info.pImageIndices = &wd.FrameIndex;

  VkResult res = vkQueuePresentKHR(queue_, &info);
  if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) {
    swap_chain_rebuild_ = true;
  }
  if (res != VK_SUCCESS) {
    TE_ERROR("Error queueing image for presentation (QueuePresent)");
    return res;
  }

  wd.SemaphoreIndex = (wd.SemaphoreIndex + 1) % wd.SemaphoreCount;
  return VK_SUCCESS;
}

void VulkanContext::ResizeSwapChain(int width, int height) {
  if (swap_chain_rebuild_ || wd.Width != width || wd.Height != height) {
    ImGui_ImplVulkan_SetMinImageCount(min_image_count_);
    ImGui_ImplVulkanH_CreateOrResizeWindow(instance_, physical_device_, device_,
                                           &wd, queue_family_, nullptr, width,
                                           height, min_image_count_, 0);
    wd.FrameIndex = 0;
    swap_chain_rebuild_ = false;
  }
}

VkResult VulkanContext::Terminate() {
  ImGui_ImplVulkanH_DestroyWindow(instance_, device_, &wd, nullptr);

  if (descriptor_pool_ == VK_NULL_HANDLE) {
    TE_WARN("Attemted to terminate null Vulkan descriptor pool");
    return VK_ERROR_INITIALIZATION_FAILED;
  }

  vkDestroyDescriptorPool(device_, descriptor_pool_, nullptr);

  if (device_ == VK_NULL_HANDLE) {
    TE_WARN("Attemted to terminate null Vulkan device");
    return VK_ERROR_INITIALIZATION_FAILED;
  }

  vkDestroyDevice(device_, nullptr);

  if (instance_ == VK_NULL_HANDLE) {
    TE_WARN("Attemted to terminate null Vulkan instance");
    return VK_ERROR_INITIALIZATION_FAILED;
  }

  vkDestroyInstance(instance_, nullptr);

  TE_TRACE("Vulkan successfully terminated");
  return VK_SUCCESS;
}
