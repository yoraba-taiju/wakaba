//
// Created by kaede on 2019/12/14.
//

#include "Vulkan.hpp"
#include "GraphicsPipelineBuilder.hpp"
#include "shader/VertexShader.hpp"
#include "ShaderModule.hpp"

namespace vk {

std::shared_ptr<GraphicsPipeline> GraphicsPipelineBuilder::build() {
  // TODO: いろいろ足さんとあかん
  VkPipeline obj;
  std::vector<VkPipelineShaderStageCreateInfo> stages = buildStages();
  VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
  VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
      .sType =  VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
      // stages
      .stageCount = static_cast<uint32_t>(stages.size()),
      .pStages = stages.data(),
      // vertex
      .pVertexInputState = nullptr,
      // fragment

      // unused fields
      .basePipelineHandle = nullptr,
      .basePipelineIndex = 0,
  };
  if(this->vertexShader_) {
    vertexInputStateCreateInfo = VkPipelineVertexInputStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0, /* flags is reserved for future use.*/
    };
    pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
  }
  // https://vulkan.lunarg.com/doc/view/1.0.33.0/linux/vkspec.chunked/ch09s02.html
  VkResult result = vkCreateGraphicsPipelines(
      vulkan_->device(),
      nullptr,
      1, // 一個しか作らない。複数作ることもできる。
      &pipelineCreateInfo,
      nullptr,
      &obj);
  return util::make_shared<GraphicsPipeline>(vulkan_, obj);
}

std::vector<VkPipelineShaderStageCreateInfo> GraphicsPipelineBuilder::buildStages() {
  std::vector<VkPipelineShaderStageCreateInfo> stages;
  if(vertexShader_) {
    stages.emplace_back(VkPipelineShaderStageCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertexShader_->module()->obj(),
        .pName = vertexShader_->module()->name().c_str(),
        .pSpecializationInfo = nullptr,
    });
  }
  return std::move(stages);
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::addVertexStage(std::shared_ptr<VertexShader> const& shader) {
  this->vertexShader_ = shader;
  return *this;
}

}
