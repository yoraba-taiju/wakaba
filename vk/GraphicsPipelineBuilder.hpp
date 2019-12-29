//
// Created by kaede on 2019/12/14.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "GraphicsPipeline.hpp"
#include "../util/Shared.hpp"

namespace vk {

class Vulkan;
class Shader;
class VertexShader;

class GraphicsPipelineBuilder final {
private:
  std::shared_ptr<Vulkan> vulkan_;
private:
  std::shared_ptr<Shader> vertexShader_;
  std::shared_ptr<Shader> fragmentShader_;
public:
  std::shared_ptr<GraphicsPipeline> build();

public:
  explicit GraphicsPipelineBuilder(std::shared_ptr<Vulkan> vulkan)
  :vulkan_(std::move(vulkan))
  {
  }

private:
  std::vector<VkPipelineShaderStageCreateInfo> buildStages();
public:
  GraphicsPipelineBuilder& addVertexStage(std::shared_ptr<VertexShader> const& shader);

public:
  ENABLE_SHARED_HELPER
};

}
