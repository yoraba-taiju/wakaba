/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "../../vk/shader/FragmentShader.hpp"
#include "../../vk/shader/VertexShader.hpp"

namespace vk {
class Device;
class ShaderModule;
}

namespace taiju::shaders {

class Triangle {
public:
  Triangle();
public:
  class Frag final : public vk::FragmentShader {
  public:
    explicit Frag(std::shared_ptr<vk::Device> const& device, vk::ShaderModule&& module);
    ~Frag() noexcept override = default;
  };
  class Vert final : public vk::VertexShader {
  public:
    struct Input {
      glm::vec2 pos;
      glm::vec3 color;
    };
  public:
    explicit Vert(std::shared_ptr<vk::Device> const& device, vk::ShaderModule&& module);
    ~Vert() noexcept override = default;
    std::tuple<
        std::vector<VkVertexInputBindingDescription>,
        std::vector<VkVertexInputAttributeDescription>> inputDescription() override;
  };
};

}
