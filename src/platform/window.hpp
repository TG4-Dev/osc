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

  void Init(windowOpts opts);
  void Destroy();

  GLFWwindow *window_;
};

} // namespace platform
