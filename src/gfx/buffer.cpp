#include "gfx/buffer.hpp"
#include "gfx/vulkan-context.hpp"
#include "spdlog/fmt/bundled/base.h"
#include <cstdint>
#include <cstring>
#include <vulkan/vulkan_core.h>

namespace gfx {

Buffer::~Buffer() { Destroy(); }

Buffer::Buffer(Buffer &&other) noexcept
    : device_(other.device_), buffer_(other.buffer_), memory_(other.memory_),
      size_(other.size_) {
  other.buffer_ = VK_NULL_HANDLE;
  other.memory_ = VK_NULL_HANDLE;
}

Buffer &Buffer::operator=(Buffer &&other) noexcept {
  if (this != &other) {
    Destroy();
    device_ = other.device_;
    buffer_ = other.buffer_;
    memory_ = other.memory_;
    size_ = other.size_;

    other.buffer_ = VK_NULL_HANDLE;
    other.memory_ = VK_NULL_HANDLE;
  }
  return *this;
};
VkResult Buffer::Create(VulkanContext &context, VkDeviceSize size,
                        VkBufferUsageFlags usageFlags,
                        VkMemoryPropertyFlags propertyFlags) {

  this->device_ = context.GetDevice();
  this->size_ = size;

  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size_;
  bufferInfo.usage = usageFlags;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VkResult res = vkCreateBuffer(device_, &bufferInfo, nullptr, &buffer_);
  if (res != VK_SUCCESS) {
    return res;
  }

  // Getting memory requirenments
  VkMemoryRequirements memoryRequirements{};
  vkGetBufferMemoryRequirements(device_, buffer_, &memoryRequirements);

  // Allocating memory
  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memoryRequirements.size;

  allocInfo.memoryTypeIndex =
      FindMemoryType(context, memoryRequirements.memoryTypeBits, propertyFlags);

  res = vkAllocateMemory(device_, &allocInfo, nullptr, &memory_);
  if (res != VK_SUCCESS) {
    return res;
  }

  vkBindBufferMemory(device_, buffer_, memory_, 0);

  return res;
}

void Buffer::Destroy() {
  if (buffer_ != VK_NULL_HANDLE) {
    vkDestroyBuffer(device_, buffer_, nullptr);
    buffer_ = VK_NULL_HANDLE;
  }
  if (memory_ != VK_NULL_HANDLE) {
    vkFreeMemory(device_, memory_, nullptr);
    memory_ = VK_NULL_HANDLE;
  }
}

void Buffer::UploadData(const void *data, VkDeviceSize dataSize) {
  // Mapping memory
  void *mappedData{};
  vkMapMemory(device_, memory_, 0, dataSize, 0, &mappedData);

  memcpy(mappedData, data, (size_t)dataSize);

  vkUnmapMemory(device_, memory_);
}

uint32_t Buffer::FindMemoryType(VulkanContext &context, uint32_t typeFilter,
                                VkMemoryPropertyFlags propertyFlags) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(context.GetPhysicalDevice(),
                                      &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {

    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &
                                    propertyFlags) == propertyFlags) {
      return i;
    }
  }
}
} // namespace gfx
