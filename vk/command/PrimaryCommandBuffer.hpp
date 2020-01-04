/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include "CommandBuffer.hpp"

namespace vk {

class SecondaryCommandBuffer;

class PrimaryCommandBuffer : public CommandBuffer {
public:
  explicit PrimaryCommandBuffer(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, VkCommandBuffer vkCommandBuffer)
  :CommandBuffer(std::move(device), std::move(commandPool), vkCommandBuffer)
  {
  }
  ~PrimaryCommandBuffer() noexcept = default;
public:
  PrimaryCommandBuffer() = delete;
  PrimaryCommandBuffer(PrimaryCommandBuffer const&) = delete;
  PrimaryCommandBuffer(PrimaryCommandBuffer&&) = default;
  PrimaryCommandBuffer& operator=(PrimaryCommandBuffer const&) = delete;
  PrimaryCommandBuffer& operator=(PrimaryCommandBuffer&&) = default;
public:
  PrimaryCommandBuffer& recordOneshot(std::function<void()> const& f);
  PrimaryCommandBuffer& record(std::function<void()> const& f);
  PrimaryCommandBuffer& record(SecondaryCommandBuffer& secondaryBuffer);
  PrimaryCommandBuffer& recordRenderPass(Framebuffer& framebuffer, std::function<void()> const& f);
  PrimaryCommandBuffer& recordRenderPass(Framebuffer& framebuffer, std::vector<SecondaryCommandBuffer>& cmds);
public:
  PrimaryCommandBuffer& copyBufferSync(Buffer& dst, VkDeviceSize dstOffset, Buffer& src, VkDeviceSize srcOffset, VkDeviceSize size);
  PrimaryCommandBuffer& executeSync();
};

}
