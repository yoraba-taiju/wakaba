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
#include <utility>

namespace vk {

class Device;
class Queue;

class CommandBuffer;

class CommandPool final : public std::enable_shared_from_this<CommandPool> {
private:
  std::shared_ptr<Device> device_;
  VkQueue queue_;
  VkCommandPool vkCommandPool_;
public:
  CommandPool() = delete;
  CommandPool(CommandPool const&) = delete;
  CommandPool(CommandPool&&) = delete;
  CommandPool& operator=(CommandPool const&) = delete;
  CommandPool& operator=(CommandPool&&) = delete;

  explicit CommandPool(std::shared_ptr<Device> device, VkQueue queue, VkCommandPool vkCommandPool)
  :device_(std::move(device))
  ,queue_(queue)
  ,vkCommandPool_(vkCommandPool)
  {
  }
  ~CommandPool() noexcept;

public:
  [[ nodiscard ]] VkCommandPool vkCommandPool() {
    return this->vkCommandPool_;
  }

public:
  CommandBuffer createBuffer();
};

}
