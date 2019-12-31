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
class RenderPass;
class Shader;
class VertexShader;
class FragmentShader;

class GraphicsPipelineBuilder final : public std::enable_shared_from_this<GraphicsPipelineBuilder> {
private:
  std::shared_ptr<Vulkan> vulkan_;
private:
  std::shared_ptr<RenderPass> renderPass_;
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
  std::shared_ptr<GraphicsPipelineBuilder> self();
  std::shared_ptr<GraphicsPipelineBuilder> enableAlphaBlending();
  std::shared_ptr<GraphicsPipelineBuilder> disableAlphaBlending();

public:
  explicit GraphicsPipelineBuilder(std::shared_ptr<Vulkan> vulkan, std::shared_ptr<RenderPass> renderPass);
  GraphicsPipelineBuilder(GraphicsPipelineBuilder&&) = delete;
  GraphicsPipelineBuilder(GraphicsPipelineBuilder const&) = delete;
  GraphicsPipelineBuilder& operator=(GraphicsPipelineBuilder&&) = delete;
  GraphicsPipelineBuilder& operator=(GraphicsPipelineBuilder const&) = delete;
private:
  std::vector<VkPipelineShaderStageCreateInfo> buildStages();
  std::shared_ptr<PipelineLayout> buildPipelineLayout();
public:
  std::shared_ptr<GraphicsPipelineBuilder> setRenderPass(std::shared_ptr<RenderPass> renderPass);
  std::shared_ptr<GraphicsPipelineBuilder> addVertexStage(std::shared_ptr<VertexShader> shader);
  std::shared_ptr<GraphicsPipelineBuilder> addFragmentStage(std::shared_ptr<FragmentShader> shader);
};

}
