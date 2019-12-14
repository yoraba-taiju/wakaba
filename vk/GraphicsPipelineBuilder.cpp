//
// Created by kaede on 2019/12/14.
//

#include "Vulkan.hpp"
#include "GraphicsPipelineBuilder.hpp"

namespace vk {

std::shared_ptr<GraphicsPipeline> GraphicsPipelineBuilder::build() {
  // TODO: いろいろ足さんとあかん
  VkPipeline obj;
  VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo{
    .sType =  VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    // stages
    .stageCount = static_cast<uint32_t>(this->stages_.size()),
    .pStages = this->stages_.data(),
    // fill

    // unused fields
    .basePipelineHandle = nullptr,
    .basePipelineIndex = 0,
  };
  // https://vulkan.lunarg.com/doc/view/1.0.33.0/linux/vkspec.chunked/ch09s02.html
  VkResult result = vkCreateGraphicsPipelines(
      vulkan_->device(),
      nullptr,
      1, // 一個しか作らない。複数作ることもできる。
      &graphicsPipelineCreateInfo,
      nullptr,
      &obj);
  return util::make_shared<GraphicsPipeline>(vulkan_, obj);
}

}
