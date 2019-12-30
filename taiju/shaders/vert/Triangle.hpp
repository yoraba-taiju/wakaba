/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>

#include "../../../vk/shader/VertexShader.hpp"
#include "../../../util/Shared.hpp"

namespace vk {
class Vulkan;
class ShaderModule;
}

namespace taiju::shaders::vert {

class Triangle final : public vk::VertexShader {
public:
  explicit Triangle(std::shared_ptr<vk::Vulkan> const& vulkan, std::shared_ptr<vk::ShaderModule> module);
  ~Triangle() noexcept override = default;
};

}
