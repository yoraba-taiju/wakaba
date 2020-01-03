/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>

#include "../../../vk/shader/FragmentShader.hpp"

namespace vk {
class Device;
class ShaderModule;
}

namespace taiju::shaders::frag {

class Triangle final : public vk::FragmentShader {
public:
  explicit Triangle(std::shared_ptr<vk::Device> const& device, vk::ShaderModule&& module);
  ~Triangle() noexcept override = default;
};

}
