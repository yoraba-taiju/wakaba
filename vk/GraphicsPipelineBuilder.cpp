//
// Created by kaede on 2019/12/14.
//

#include "Vulkan.hpp"
#include "GraphicsPipelineBuilder.hpp"
#include "shader/VertexShader.hpp"
#include "shader/FragmentShader.hpp"
#include "ShaderModule.hpp"
#include "PipelineLayout.hpp"

namespace vk {

GraphicsPipelineBuilder::GraphicsPipelineBuilder(std::shared_ptr<Vulkan> vulkan)
:vulkan_(std::move(vulkan))
{
  // https://vulkan-tutorial.com/en/Drawing_a_triangle/Graphics_pipeline_basics/Fixed_functions
  vertexInputInfo_ = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0, // flags is reserved for future use.
      .vertexBindingDescriptionCount = 0,
      .pVertexBindingDescriptions = nullptr,
      .vertexAttributeDescriptionCount = 0,
      .pVertexAttributeDescriptions = nullptr,
  };
  inputAssemblyInfo_ = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0, // flags is reserved for future use.
      .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
      .primitiveRestartEnable = VK_FALSE,
  };
  viewport_ = {
      .x = 0.0f,
      .y = 0.0f,
      .width = static_cast<float>(vulkan_->width()),
      .height = static_cast<float>(vulkan_->height()),
      .minDepth = 0.0f,
      .maxDepth = 0.0f,
  };
  scissor_ = {
      .offset = {
          .x = 0,
          .y = 0,
      },
      .extent = {
          .width = vulkan_->width(),
          .height = vulkan_->height(),
      },
  };
  viewportInfo_ = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0, // flags is reserved for future use.
      .viewportCount = 1,
      .pViewports = &this->viewport_,
      .scissorCount = 1,
      .pScissors = &this->scissor_,
  };
  rasterizerInfo_ = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0, // flags is reserved for future use.
      /*
       * If depthClampEnable is set to VK_TRUE,
       * then fragments that are beyond the near and far planes are clamped to them as opposed to discarding them.
       * This is useful in some special cases like shadow maps.
       */
      .depthClampEnable = VK_FALSE,
      .rasterizerDiscardEnable = VK_FALSE,
      .polygonMode = VK_POLYGON_MODE_FILL,
      .cullMode = VK_CULL_MODE_BACK_BIT,
      .frontFace = VK_FRONT_FACE_CLOCKWISE,
      .depthBiasEnable = VK_FALSE,
      .depthBiasConstantFactor = 0.0f, // Optional
      .depthBiasClamp = 0.0f, // Optional
      .depthBiasSlopeFactor = 0.0f, // Optional
      .lineWidth = 1.0f,
  };
  multisamplingInfo_ = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0, // flags is reserved for future use.
      .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
      .sampleShadingEnable = VK_FALSE,
      .minSampleShading = 1.0f, // Optional
      .pSampleMask = nullptr, // Optional
      .alphaToCoverageEnable = VK_FALSE, // Optional
      .alphaToOneEnable = VK_FALSE, // Optional
  };
  this->disableAlphaBlending();
  colorBlendingInfo_ = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0, // flags is reserved for future use.
      .logicOpEnable = VK_FALSE,
      .logicOp = VK_LOGIC_OP_COPY, // Optional
      .attachmentCount = 1,
      .pAttachments = &this->colorBlender_,
      .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}, // Optional
  };

  this->dynamicStates_ = {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_LINE_WIDTH
  };
  dynamicStateInfo_ = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0, // flags is reserved for future use.
      .dynamicStateCount = static_cast<uint32_t>(dynamicStates_.size()),
      .pDynamicStates = dynamicStates_.data(),
  };
}

std::shared_ptr<GraphicsPipeline> GraphicsPipelineBuilder::build() {
  VkPipeline obj;
  std::vector<VkPipelineShaderStageCreateInfo> stages = buildStages();
  std::shared_ptr<PipelineLayout> pipelineLayout = buildPipelineLayout();
  VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
      .sType =  VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0, /* flags is reserved for future use.*/
      // stages
      .stageCount = static_cast<uint32_t>(stages.size()),
      .pStages = stages.data(),
      // vertex
      .pVertexInputState = &this->vertexInputInfo_,
      .pInputAssemblyState = &this->inputAssemblyInfo_,
      .pTessellationState = nullptr,
      .pViewportState = &this->viewportInfo_,
      .pRasterizationState = &this->rasterizerInfo_,
      .pMultisampleState = &this->multisamplingInfo_,
      .pDepthStencilState = nullptr, // FIXME
      .pColorBlendState = &this->colorBlendingInfo_,
      .pDynamicState = &this->dynamicStateInfo_,
      .layout = pipelineLayout->obj(),
      .renderPass = nullptr, // FIXME
      .subpass = 0,
      // unused fields
      .basePipelineHandle = nullptr,
      .basePipelineIndex = -1,
  };

  // https://vulkan.lunarg.com/doc/view/1.0.33.0/linux/vkspec.chunked/ch09s02.html
  // 一個しか作らない。複数作ることもできる。
  if(vkCreateGraphicsPipelines(vulkan_->device(), nullptr, 1, &pipelineCreateInfo, nullptr, &obj) != VK_SUCCESS) {
    vulkan_->log().fatal("Failed to create graphics pipeline.");
  }
  return util::make_shared<GraphicsPipeline>(vulkan_, obj);
}

void GraphicsPipelineBuilder::enableAlphaBlending() {
  this->colorBlender_ = {
      .blendEnable = VK_TRUE,
      .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .colorBlendOp = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp = VK_BLEND_OP_ADD,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
  };
}

void GraphicsPipelineBuilder::disableAlphaBlending() {
  this->colorBlender_ = {
      .blendEnable = VK_FALSE,
      .srcColorBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
      .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
      .colorBlendOp = VK_BLEND_OP_ADD, // Optional
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
      .alphaBlendOp = VK_BLEND_OP_ADD, // Optional
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
  };
}

std::vector<VkPipelineShaderStageCreateInfo> GraphicsPipelineBuilder::buildStages() {
  std::vector<VkPipelineShaderStageCreateInfo> stages;
  if(vertexShader_) {
    stages.emplace_back(VkPipelineShaderStageCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertexShader_->module()->obj(),
        .pName = vertexShader_->module()->name().c_str(),
        .pSpecializationInfo = nullptr,
    });
  }
  if(fragmentShader_) {
    stages.emplace_back(VkPipelineShaderStageCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fragmentShader_->module()->obj(),
        .pName = fragmentShader_->module()->name().c_str(),
        .pSpecializationInfo = nullptr,
    });
  }
  return std::move(stages);
}

std::shared_ptr<PipelineLayout> GraphicsPipelineBuilder::buildPipelineLayout() {
  VkPipelineLayout pipelineLayout;
  VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .setLayoutCount = 0, // Optional
      .pSetLayouts = nullptr, // Optional
      .pushConstantRangeCount = 0, // Optional
      .pPushConstantRanges = nullptr, // Optional
  };

  if (vkCreatePipelineLayout(vulkan_->device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
    vulkan_->log().fatal("failed to create pipeline layout!");
  }
  return util::make_shared<PipelineLayout>(vulkan_, pipelineLayout);
}

void GraphicsPipelineBuilder::addVertexStage(std::shared_ptr<VertexShader> const& shader) {
  this->vertexShader_ = shader;
}

void GraphicsPipelineBuilder::addFragmentStage(std::shared_ptr<FragmentShader> const& shader) {
  this->fragmentShader_ = shader;
}


}
