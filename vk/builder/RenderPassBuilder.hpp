/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <memory>
#include <vector>
#include <optional>

namespace vk {

class Vulkan;
class RenderPass;
class SubPassBuilder;
class AttachmentBuilder;
class SubPassDependencyBuilder;

class RenderPassBuilder final {
private:
  std::shared_ptr<Vulkan> vulkan_;

public:
  explicit RenderPassBuilder(std::shared_ptr<Vulkan> vulkan);
  RenderPassBuilder() = delete;
  RenderPassBuilder(RenderPassBuilder&&) = delete;
  RenderPassBuilder(RenderPassBuilder const&) = delete;
  RenderPassBuilder& operator=(RenderPassBuilder&&) = delete;
  RenderPassBuilder& operator=(RenderPassBuilder const&) = delete;

private:
  std::vector<AttachmentBuilder> attachments_;
  std::vector<SubPassBuilder> subPasses_;
  std::vector<SubPassDependencyBuilder> dependencies_;
private:
  VkRenderPassCreateInfo renderPassInfo_{};
public:
  RenderPass build();

public:
  AttachmentBuilder& addAttachment(VkFormat format);
  SubPassBuilder& addSubPass();
};

class AttachmentBuilder {
private:
  VkAttachmentDescription vkAttachmentDescription_{};
public:
  AttachmentBuilder() = delete;
  explicit AttachmentBuilder(VkFormat format);
  VkAttachmentDescription build();
};

class SubPassBuilder final {
private:
  VkSubpassDescription vkSubpassDescription_{};
  std::vector<VkAttachmentReference> inputs_;
  std::vector<VkAttachmentReference> colors_;
  std::optional<VkAttachmentReference> depthStencil_;
  std::vector<uint32_t> preserves_;
public:
  explicit SubPassBuilder(VkPipelineBindPoint vkPipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);
  VkSubpassDescription build();

public:
  SubPassBuilder& addInput(uint32_t location, VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
  SubPassBuilder& addColor(uint32_t location, VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
  SubPassBuilder& setDepthStencil(uint32_t location, VkImageLayout layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  SubPassBuilder& preserve(uint32_t location);
};

class SubPassDependencyBuilder {
private:
  VkSubpassDependency vkSubpassDependency_{};
public:
  SubPassDependencyBuilder();
  VkSubpassDependency build();
};

}

