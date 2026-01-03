#pragma once

#include "gfx/vulkan-context.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace gfx {
class Buffer {
public:
  Buffer() = default;
  ~Buffer();
  // No copy
  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;

  // Move
  Buffer(Buffer &&other) noexcept;
  Buffer &operator=(Buffer &&other) noexcept;
  VkResult Create(VulkanContext &context, VkDeviceSize size,
                  VkBufferUsageFlags usageFlags,
                  VkMemoryPropertyFlags propertyFlags);

  void Destroy();

  void UploadData(const void *data, VkDeviceSize size);

  VkBuffer GetBuffer() const { return buffer_; }
  VkDeviceMemory GetMemory() const { return memory_; }
  VkDevice GetDevice() const { return device_; }

private:
  VkBuffer buffer_ = VK_NULL_HANDLE;
  VkDeviceMemory memory_ = VK_NULL_HANDLE;
  VkDevice device_ = VK_NULL_HANDLE;

  VkDeviceSize size_ = 0;

  uint32_t FindMemoryType(VulkanContext &context, uint32_t typeFilter,
                          VkMemoryPropertyFlags propertyFlags);
};
} // namespace gfx
