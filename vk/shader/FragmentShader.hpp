//
// Created by kaede on 2019/12/30.
//

#pragma once

#include <memory>
#include <utility>
#include "../../util/Shared.hpp"
#include "../ShaderModule.hpp"
#include "Shader.hpp"

namespace vk {

class Vulkan;
class ShaderModule;

class FragmentShader : public Shader {
public:
  FragmentShader(FragmentShader&&) = delete;
  FragmentShader(FragmentShader const&) = delete;
  FragmentShader& operator=(FragmentShader&&) = delete;
  FragmentShader& operator=(FragmentShader const&) = delete;

protected:
  explicit FragmentShader(std::shared_ptr<Vulkan> const& vulkan, std::shared_ptr<ShaderModule> module)
  :Shader(vulkan,std::move(module))
  {
  }

public:
  ~FragmentShader() noexcept override;
};

}
