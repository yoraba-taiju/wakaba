/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../GraphicsPipeline.hpp"
#include "../../util/Shared.hpp"

namespace vk {

class Vulkan;
class Shader;
class VertexShader;
class FragmentShader;

class GraphicsPipelineBuilder final {
private:
  std::shared_ptr<Vulkan> vulkan_;
private:
  std::shared_ptr<Shader> vertexShader_{};
  std::shared_ptr<Shader> fragmentShader_{};
private: // input info
  VkPipelineVertexInputStateCreateInfo vertexInputInfo_{};
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo_{};
  VkViewport viewport_{};
  VkRect2D scissor_{};
  VkPipelineViewportStateCreateInfo viewportInfo_{};
  VkPipelineRasterizationStateCreateInfo rasterizerInfo_{};
  VkPipelineMultisampleStateCreateInfo multisamplingInfo_{};
  //VkPipelineDepthStencilStateCreateInfo depthStencilInfo_{};
  VkPipelineColorBlendAttachmentState colorBlender_{};
  VkPipelineColorBlendStateCreateInfo colorBlendingInfo_{};

  std::array<VkDynamicState, 2> dynamicStates_{};
  VkPipelineDynamicStateCreateInfo dynamicStateInfo_{};
public:
  std::shared_ptr<GraphicsPipeline> build();

public:
  void enableAlphaBlending();
  void disableAlphaBlending();

public:
  explicit GraphicsPipelineBuilder(std::shared_ptr<Vulkan> vulkan);
private:
  std::vector<VkPipelineShaderStageCreateInfo> buildStages();
  std::shared_ptr<PipelineLayout> buildPipelineLayout();
public:
  void addVertexStage(std::shared_ptr<VertexShader> const& shader);
  void addFragmentStage(std::shared_ptr<FragmentShader> const& shader);

public:
  ENABLE_SHARED_HELPER
};

}
