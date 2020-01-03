/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "RenderPass.hpp"
#include "Vulkan.hpp"

namespace vk {

RenderPass::RenderPass(std::shared_ptr<Device> device, VkRenderPass renderPass)
:device_(std::move(device))
,vkRenderPass_(renderPass)
{
}

RenderPass::~RenderPass() noexcept{
  device_->destroyRenderPass(*this);
}

}
