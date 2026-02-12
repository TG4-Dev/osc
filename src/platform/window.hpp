#pragma once
#include <GLFW/glfw3.h>

namespace platform {

struct windowOpts {
  int width;
  int height;
  const char *name = nullptr;
};

class Window {
public:
  Window();
  ~Window();

  int Init(windowOpts opts);
  void Destroy();

  GLFWwindow *GetWindowHandle() { return window_; }

  float main_scale;

private:
  GLFWwindow *window_;
};

} // namespace platform
