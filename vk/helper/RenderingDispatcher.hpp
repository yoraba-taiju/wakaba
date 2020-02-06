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
#include "../command/SecondaryCommandBuffer.hpp"

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
  struct FrameSyncInfo final {
    uint32_t imageIndex_{};
    VkSemaphore renderFinishedSemaphore_{};
    VkSemaphore imageAvailableSemaphore_{};
    VkFence fence_{};
    std::shared_ptr<CommandPool> commandPool_{};
    std::vector<PrimaryCommandBuffer> primaryBuffers_{};
    std::vector<SecondaryCommandBuffer> secondaryBuffers_{};
    void clear() noexcept {
      primaryBuffers_.clear();
      secondaryBuffers_.clear();
    }
  };
  std::array<FrameSyncInfo, NumFrames> syncInfo_{};
private:
  uint32_t currentFrame_{};
private:
  explicit RenderingDispatcher(std::shared_ptr<Device> device, std::shared_ptr<Swapchain> swapchain);

public:
  RenderingDispatcher() = delete;
  RenderingDispatcher(RenderingDispatcher const&) = delete;
  RenderingDispatcher(RenderingDispatcher&&) = default;
  RenderingDispatcher& operator=(RenderingDispatcher const&) = delete;
  RenderingDispatcher& operator=(RenderingDispatcher&&) = default;
  ~RenderingDispatcher() noexcept;

public:
  RenderingDispatcher& submit(PrimaryCommandBuffer&& commandBuffer);
  RenderingDispatcher& submit(SecondaryCommandBuffer&& commandBuffer);

public:
  void dispatch(std::function<void(std::shared_ptr<CommandPool> const&, uint32_t)> const& f);
};

}


