#pragma once
#include "platform/window.hpp"

namespace core {

class Application{
	public:
		Application(platform::windowOpts);
		~Application();

		void Run();

		void Stop();

		void Update();

		platform::Window window;
	private:
		bool isRunning = false;
};

} //core
