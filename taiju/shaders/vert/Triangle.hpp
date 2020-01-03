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
class Device;
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
  explicit Triangle(std::shared_ptr<vk::Device> const& device, vk::ShaderModule&& module);
  ~Triangle() noexcept override = default;
  std::tuple<
      std::vector<VkVertexInputBindingDescription>,
      std::vector<VkVertexInputAttributeDescription>> inputDescription() override;
};

}
