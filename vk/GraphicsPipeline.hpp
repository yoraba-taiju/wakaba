//
// Created by kaede on 2019/12/14.
//

#pragma once

#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../util/Shared.hpp"
#include "Pipeline.hpp"

namespace vk {

class Vulkan;
class GraphicsPipelineBuilder;

class GraphicsPipeline final : Pipeline {
  friend class GraphicsPipelineBuilder;
private:
  GraphicsPipeline(std::weak_ptr<Vulkan> vulkan, VkPipeline pipeline)
  : Pipeline(std::move(vulkan), pipeline)
  {

  }
public:
  ~GraphicsPipeline() noexcept = default;
public:
  ENABLE_SHARED_HELPER
};

}


