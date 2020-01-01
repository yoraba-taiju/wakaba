/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "FramebufferBuilder.hpp"
#include "../Vulkan.hpp"
#include "../Framebuffer.hpp"
#include "../RenderPass.hpp"

#include <utility>

namespace vk {

FramebufferBuilder::FramebufferBuilder(std::shared_ptr<Vulkan> const& vulkan, std::shared_ptr<RenderPass> renderPass)
:vulkan_(vulkan)
,renderPass_(std::move(renderPass))
,width_(vulkan->width())
,height_(vulkan->height())
{
}

FramebufferBuilder::FramebufferBuilder(std::shared_ptr<Vulkan> vulkan, uint32_t width, uint32_t height, std::shared_ptr<RenderPass> renderPass)
:vulkan_(std::move(vulkan))
,renderPass_(std::move(renderPass))
,width_(width)
,height_(height)
{

}

FramebufferBuilder& FramebufferBuilder::addColorImage(std::shared_ptr<Image> image) {
  this->colorImages_.emplace_back(std::move(image));
  return *this;
}

Framebuffer FramebufferBuilder::build() {
  VkFramebuffer vkFramebuffer;
  std::vector<VkImageView> attachmentViews;
  {
    VkFramebufferCreateInfo vkFramebufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .renderPass = renderPass_->vkRenderPass(),
        .attachmentCount = static_cast<uint32_t>(attachmentViews.size()),
        .pAttachments = attachmentViews.data(),
        .width = width_,
        .height = height_,
        .layers = 1,
    };
    if (vkCreateFramebuffer(vulkan_->vkDevice(), &vkFramebufferCreateInfo, nullptr, &vkFramebuffer) != VK_SUCCESS) {
      vulkan_->log().fatal("[Vulkan] Failed to create a FrameBuffer.");
    }
  }
  return Framebuffer(vulkan_, vkFramebuffer, renderPass_);
}


}
