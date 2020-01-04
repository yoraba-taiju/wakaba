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

class GraphicsPipelineBuilder;

class GraphicsPipeline final : public Pipeline {
  friend class GraphicsPipelineBuilder;

public:
  explicit GraphicsPipeline(std::shared_ptr<Device> device, VkPipeline pipeline)
  : Pipeline(std::move(device), pipeline)
  {
  }
  ~GraphicsPipeline() noexcept = default;
public:
  GraphicsPipeline() = delete;
  GraphicsPipeline(GraphicsPipeline const&) = delete;
  GraphicsPipeline& operator=(GraphicsPipeline const&) = delete;
  GraphicsPipeline(GraphicsPipeline&&) = default;
  GraphicsPipeline& operator=(GraphicsPipeline&&) = default;
};

}


