#include "gfx/buffer.hpp"
#include "gfx/vertex.hpp"
#include "gfx/vulkan-context.hpp"
#include "glm/fwd.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>
class Mesh {
public:
  void LoadFromVerticies(VulkanContext &context,
                         const std::vector<Vertex> &verticies);

  void draw(VkCommandBuffer cmd);

  void cleanup();

private:
  Buffer vertex_buffer_;
  glm::uint32_t vertex_count_;
};
