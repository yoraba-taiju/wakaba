//
// Created by kaede on 2019/12/29.
//

#pragma once

#include <memory>

#include "../../../vk/shader/FragmentShader.hpp"
#include "../../../util/Shared.hpp"

namespace vk {
class Vulkan;
class ShaderModule;
}

namespace taiju::shaders::frag {

class Triangle final : public vk::FragmentShader {
public:
  explicit Triangle(std::shared_ptr<vk::Vulkan> const& vulkan, std::shared_ptr<vk::ShaderModule> module);
  ~Triangle() noexcept override = default;
};

}
