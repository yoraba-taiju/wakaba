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

class GraphicsPipelineBuilder final {
private:
  std::shared_ptr<Vulkan> vulkan_;
private:
  std::vector<VkPipelineShaderStageCreateInfo> stages_;
public:
  std::shared_ptr<GraphicsPipeline> build();

public:
  ENABLE_SHARED_HELPER
};

}
