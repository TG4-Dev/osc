#include "application.hpp"
#include "platform/platform.hpp"

namespace core {

	Application::Application(platform::windowOpts opts) {
		platform::Init();

		window.Init(opts);
	}

	Application::~Application() {
		window.Destroy();
		platform::Exit();
	}

	void Application::Run() {
		isRunning = true;

		while(isRunning) {
			glfwPollEvents();

			if(glfwWindowShouldClose(window.window))
				Stop();
		}
	}

	void Application::Stop() {
		isRunning = false;
	}
} //namespace core
