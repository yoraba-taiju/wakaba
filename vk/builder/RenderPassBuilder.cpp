/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "RenderPassBuilder.hpp"
#include "../Vulkan.hpp"
#include "../RenderPass.hpp"

namespace vk {

RenderPassBuilder::RenderPassBuilder(std::shared_ptr<Vulkan> vulkan)
:vulkan_(std::move(vulkan))
{
  renderPassInfo_ = {
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0, // reserved
  };
}

std::shared_ptr<RenderPass> RenderPassBuilder::build() {
  VkRenderPass pass{};
  VkRenderPassCreateInfo renderPassInfo = renderPassInfo_;
  std::vector<VkAttachmentDescription> attachments;
  { // attatchments
    for(AttachmentBuilder& attachment : this->attachments_) {
      attachments.emplace_back(attachment.build());
    }
    renderPassInfo.attachmentCount = attachments.size();
    renderPassInfo.pAttachments = attachments.data();
  }
  std::vector<VkSubpassDescription> subPasses;
  { //sub passes
    for(SubPassBuilder& subPass : this->subPasses_) {
      subPasses.emplace_back(subPass.build());
    }
    renderPassInfo.subpassCount = subPasses.size();
    renderPassInfo.pSubpasses = subPasses.data();
  }
  std::vector<VkSubpassDependency> dependencies;
  // dependencies
  if(this->dependencies_.empty()) {
    renderPassInfo.dependencyCount = 0;
    renderPassInfo.pDependencies = nullptr;
  } else {
    for(SubPassDependencyBuilder& dependency : this->dependencies_) {
      dependencies.emplace_back(dependency.build());
    }
    renderPassInfo.dependencyCount = dependencies.size();
    renderPassInfo.pDependencies = dependencies.data();
  }
  if (vkCreateRenderPass(vulkan_->vkDevice(), &renderPassInfo, nullptr, &pass) != VK_SUCCESS) {
    vulkan_->log().fatal("[Vulkan] Failed to create a RenderPass.");
  }
  return std::make_shared<RenderPass>(vulkan_, pass);
}

AttachmentBuilder& RenderPassBuilder::addAttachment(VkFormat format) {
  this->attachments_.emplace_back(format);
  return this->attachments_.back();
}

SubPassBuilder& RenderPassBuilder::addSubPass() {
  this->subPasses_.emplace_back();
  return this->subPasses_.back();
}

AttachmentBuilder::AttachmentBuilder(VkFormat format) {
  vkAttachmentDescription_ = {
    .format = format,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
    .initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
  };
}

VkAttachmentDescription AttachmentBuilder::build() {
  return this->vkAttachmentDescription_;
}

SubPassBuilder::SubPassBuilder(VkPipelineBindPoint const vkPipelineBindPoint) {
  vkSubpassDescription_ = {
      .flags = 0,
      .pipelineBindPoint = vkPipelineBindPoint,
      .inputAttachmentCount = static_cast<uint32_t>(this->inputs_.size()),
      .pInputAttachments = this->inputs_.data(),
      .colorAttachmentCount = static_cast<uint32_t>(this->colors_.size()),
      .pColorAttachments = this->colors_.data(),
      .pResolveAttachments = nullptr,
      .pDepthStencilAttachment = this->depthStencil_.has_value() ? &this->depthStencil_.value() : nullptr,
      .preserveAttachmentCount = static_cast<uint32_t>(this->preserves_.size()),
      .pPreserveAttachments = this->preserves_.data(),
  };
}

VkSubpassDescription SubPassBuilder::build() {
  vkSubpassDescription_.inputAttachmentCount = this->inputs_.size();
  vkSubpassDescription_.pInputAttachments = this->inputs_.data();
  vkSubpassDescription_.colorAttachmentCount = this->colors_.size();
  vkSubpassDescription_.pColorAttachments = this->colors_.data();
  vkSubpassDescription_.preserveAttachmentCount = this->preserves_.size();
  vkSubpassDescription_.pPreserveAttachments = this->preserves_.data();
  return this->vkSubpassDescription_;
}

SubPassBuilder& SubPassBuilder::addInput(uint32_t const location, VkImageLayout const layout) {
  this->inputs_.emplace_back(VkAttachmentReference {
    .attachment = location,
    .layout = layout,
  });
  return *this;
}

SubPassBuilder& SubPassBuilder::addColor(uint32_t const location, VkImageLayout const layout) {
  this->colors_.emplace_back(VkAttachmentReference {
      .attachment = location,
      .layout = layout,
  });
  return *this;
}

SubPassBuilder& SubPassBuilder::setDepthStencil(uint32_t const location, VkImageLayout const layout) {
  this->depthStencil_ = std::make_optional<VkAttachmentReference>(VkAttachmentReference {
      .attachment = location,
      .layout = layout,
  });
  return *this;
}

SubPassBuilder &SubPassBuilder::preserve(uint32_t const location) {
  this->preserves_.emplace_back(location);
  return *this;
}


SubPassDependencyBuilder::SubPassDependencyBuilder() {
  this->vkSubpassDependency_ = {

  };
}

VkSubpassDependency SubPassDependencyBuilder::build() {
  return this->vkSubpassDependency_;
}

}