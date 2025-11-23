#pragma once
#include "platform/window.hpp"
#include "gfx/vulkan-test.hpp"

namespace core {

class Application{
	public:
		Application(platform::windowOpts);
		~Application();

		void Run();

		void Stop();

		void Update();

		platform::Window window;

		VulkanTest vulkanTest;
	private:
		bool isRunning = false;
};

} //core
