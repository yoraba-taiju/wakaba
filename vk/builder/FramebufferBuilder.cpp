/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include <utility>

#include "FramebufferBuilder.hpp"
#include "../Vulkan.hpp"
#include "../Framebuffer.hpp"
#include "../RenderPass.hpp"
#include "../image/Image.hpp"

namespace vk {

FramebufferBuilder::FramebufferBuilder(std::shared_ptr<Device> device, uint32_t width, uint32_t height, std::shared_ptr<RenderPass> renderPass)
:device_(std::move(device))
,renderPass_(std::move(renderPass))
,width_(width)
,height_(height)
{

}

FramebufferBuilder& FramebufferBuilder::addColor(std::shared_ptr<Image> image, std::array<float, 4> const& value) {
  this->images_.emplace_back(std::move(image));
  this->clears_.emplace_back(VkClearValue{
    .color = {value[0], value[1], value[2], value[3]},
  });
  return *this;
}

FramebufferBuilder &FramebufferBuilder::addDepth(std::shared_ptr<Image> image, float const value) {
  this->images_.emplace_back(std::move(image));
  this->clears_.emplace_back(VkClearValue{
      .depthStencil = {
          .depth = value,
      },
  });
  return *this;
}

FramebufferBuilder &FramebufferBuilder::addStencil(std::shared_ptr<Image> image, uint32_t const value) {
  this->images_.emplace_back(std::move(image));
  this->clears_.emplace_back(VkClearValue{
      .depthStencil = {
          .stencil = value
      },
  });
  return *this;
}

Framebuffer FramebufferBuilder::build() {
  std::vector<VkImageView> attachmentViews;
  for(std::shared_ptr<Image>& it : this->images_) {
    attachmentViews.emplace_back(it->vkImageView());
  }
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
  VkFramebuffer vkFramebuffer;
  if (vkCreateFramebuffer(device_->vkDevice(), &vkFramebufferCreateInfo, nullptr, &vkFramebuffer) != VK_SUCCESS) {
    throw std::runtime_error("[Vulkan] Failed to create a FrameBuffer.");
  }
  return Framebuffer(device_, vkFramebuffer, renderPass_, images_, clears_, width_, height_);
}


}
