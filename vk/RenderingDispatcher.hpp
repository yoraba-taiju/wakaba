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
#include <array>
#include <vector>

namespace vk {

class Vulkan;
class RenderingDispatcherBuilder;

class RenderingDispatcher final {
public:
  constexpr static size_t NumFrames = 2;
private:
  friend class RenderingDispatcherBuilder;
  std::shared_ptr<Vulkan> vulkan_;
  std::array<VkFence, NumFrames> fences_;
  std::array<VkSemaphore , NumFrames> renderFinishedSemaphores_;
  std::array<VkSemaphore , NumFrames> imageAvailableSemaphores_;
  std::vector<VkFence> swapchainFences_;

private:
  explicit RenderingDispatcher(std::shared_ptr<Vulkan> vulkan);

public:
  RenderingDispatcher() = delete;
  RenderingDispatcher(RenderingDispatcher const&) = delete;
  RenderingDispatcher(RenderingDispatcher&&) = default;
  RenderingDispatcher& operator=(RenderingDispatcher const&) = delete;
  RenderingDispatcher& operator=(RenderingDispatcher&&) = default;
};

}


