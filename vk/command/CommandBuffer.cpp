/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "../buffer/Buffer.hpp"
#include "../Vulkan.hpp"
#include "../GraphicsPipeline.hpp"

namespace vk {

CommandBuffer::~CommandBuffer() noexcept{
  if(device_) {
    device_->destroyCommandBuffer(*this);
  }
}

void CommandBuffer::bindPipeline(GraphicsPipeline& graphicsPipeilne) {
  vkCmdBindPipeline(vkCommandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeilne.vkPipeline());
}

void CommandBuffer::bindVertexBuffer(uint32_t const bindingPoint, Buffer& buffer) {
  VkBuffer vertexBuffers[] = {buffer.vkBuffer()};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(vkCommandBuffer_, bindingPoint, 1, vertexBuffers, offsets);
}

void CommandBuffer::draw(uint32_t const vertices, uint32_t const instances) {
  vkCmdDraw(vkCommandBuffer_, vertices, 1, 0, 0);
}

}