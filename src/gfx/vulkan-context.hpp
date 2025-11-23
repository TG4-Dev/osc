#pragma once
#include <vulkan/vulkan.h>

class VulkanTest {
	public:
		void Terminate();

		void CreateInstance();
	private:
		VkInstance instance;
};
