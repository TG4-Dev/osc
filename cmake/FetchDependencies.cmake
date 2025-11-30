include(FetchContent)


FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.16.0
)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.2
)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW tests")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW documentation")
set(GLFW_VULKAN_STATIC ON CACHE BOOL "Enable Vulkan support in GLFW")

FetchContent_Declare(
    gtest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_Declare(
  Taskflow
  GIT_REPOSITORY https://github.com/taskflow/taskflow.git
  GIT_TAG v3.11.0
)

FetchContent_MakeAvailable(
  spdlog
  glm
  glfw
  gtest
  Taskflow
)
