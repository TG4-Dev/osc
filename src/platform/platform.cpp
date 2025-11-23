#include "platform.hpp"
#include <stdexcept>

void platform::Init() {
	if(!glfwInit()) {
		throw std::runtime_error("glfw init error");
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void platform::Exit() {
	glfwTerminate();
}
