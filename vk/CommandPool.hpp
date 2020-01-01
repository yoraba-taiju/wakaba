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

class Vulkan;
class CommandBuffer;

class CommandPool final : public std::enable_shared_from_this<CommandPool> {
private:
  std::weak_ptr<Vulkan> vulkan_;
  VkCommandPool vkCommandPool_;
public:
  CommandPool() = delete;
  CommandPool(CommandPool const&) = delete;
  CommandPool(CommandPool&&) = delete;
  CommandPool& operator=(CommandPool const&) = delete;
  CommandPool& operator=(CommandPool&&) = delete;

  explicit CommandPool(std::weak_ptr<Vulkan> vulkan, VkCommandPool vkCommandPool)
  : vulkan_(std::move(vulkan))
  , vkCommandPool_(vkCommandPool)
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
