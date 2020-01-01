/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Pipeline.hpp"

namespace vk {

class Vulkan;
class GraphicsPipelineBuilder;

class GraphicsPipeline final : public Pipeline {
  friend class GraphicsPipelineBuilder;

public:
  explicit GraphicsPipeline(std::weak_ptr<Vulkan> vulkan, VkPipeline pipeline)
  : Pipeline(std::move(vulkan), pipeline)
  {
  }
public:
  ~GraphicsPipeline() noexcept = default;

};

}


