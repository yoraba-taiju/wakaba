/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include "CommandBuffer.hpp"

namespace vk {

class SecondaryCommandBuffer : public CommandBuffer {
public:
  explicit SecondaryCommandBuffer(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, VkCommandBuffer vkCommandBuffer)
  :CommandBuffer(std::move(device), std::move(commandPool), vkCommandBuffer)
  {
  }
  ~SecondaryCommandBuffer() noexcept = default;
public:
  SecondaryCommandBuffer() = delete;
  SecondaryCommandBuffer(SecondaryCommandBuffer const&) = delete;
  SecondaryCommandBuffer(SecondaryCommandBuffer&&) = default;
  SecondaryCommandBuffer& operator=(SecondaryCommandBuffer const&) = delete;
  SecondaryCommandBuffer& operator=(SecondaryCommandBuffer&&) = default;
public:
  SecondaryCommandBuffer& recordOneshot(Framebuffer &framebuffer, std::function<void()> const& f);
  SecondaryCommandBuffer& record(Framebuffer &framebuffer, std::function<void()> const& f);
};

}
