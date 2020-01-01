/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>

#include "../../../vk/shader/VertexShader.hpp"
#include <glm/glm.hpp>

namespace vk {
class Vulkan;
class ShaderModule;
}

namespace taiju::shaders::vert {

class Triangle final : public vk::VertexShader {
public:
  struct Input {
    glm::vec2 pos;
    glm::vec3 color;
  };
public:
  explicit Triangle(std::shared_ptr<vk::Vulkan> const& vulkan, vk::ShaderModule&& module);
  ~Triangle() noexcept override = default;
  std::tuple<
      std::vector<VkVertexInputBindingDescription>,
      std::vector<VkVertexInputAttributeDescription>> inputDescription() override;
};

}
