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
#include "../command/PrimaryCommandBuffer.hpp"

namespace vk {

class Device;
class RenderingDispatcherBuilder;
class Swapchain;
class Framebuffer;

class RenderingDispatcher final {
public:
  constexpr static size_t NumFrames = 2;
private:
  friend class RenderingDispatcherBuilder;
  std::shared_ptr<Device> device_{};
  std::shared_ptr<Swapchain> swapchain_{};
private:
  std::array<VkFence, NumFrames> fences_{};
  std::array<VkSemaphore , NumFrames> renderFinishedSemaphores_{};
  std::array<VkSemaphore , NumFrames> imageAvailableSemaphores_{};
  std::vector<VkFence> swapchainFences_{};
private:
  uint32_t currentFrame_{};
  uint32_t currentImageIndex_{};
  std::array<std::vector<PrimaryCommandBuffer>, NumFrames> commands_{};
  std::vector<std::vector<PrimaryCommandBuffer>> usedCommands_{};

private:
  explicit RenderingDispatcher(std::shared_ptr<Device> device, std::shared_ptr<Swapchain> swapchain);

public:
  RenderingDispatcher() = delete;
  RenderingDispatcher(RenderingDispatcher const&) = delete;
  RenderingDispatcher(RenderingDispatcher&&) = default;
  RenderingDispatcher& operator=(RenderingDispatcher const&) = delete;
  RenderingDispatcher& operator=(RenderingDispatcher&&) = default;

public:
  RenderingDispatcher& push(PrimaryCommandBuffer&& commandBuffer);

public:
  void dispatch(std::function<void(RenderingDispatcher&, uint32_t)> const& f);
};

}


