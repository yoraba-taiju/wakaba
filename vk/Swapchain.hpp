/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vk{
class Device;
class SwapchainImage;

class Swapchain final : std::enable_shared_from_this<Swapchain> {
private:
  std::shared_ptr<Device> device_;
  VkSwapchainKHR vkSwapchain_{};
  VkSurfaceFormatKHR vkSwapchainFormat_{};
  std::vector<std::shared_ptr<SwapchainImage>> images_{};
public:
  Swapchain() = delete;
  Swapchain(std::shared_ptr<Device> device, VkSwapchainKHR vkSwapchain, VkSurfaceFormatKHR vkSwapchainFormat, std::vector<std::shared_ptr<SwapchainImage>> images);
  ~Swapchain() noexcept;
  static std::shared_ptr<Swapchain> create(std::shared_ptr<Device> device, VkSwapchainKHR vkSwapchain, VkSurfaceFormatKHR vkSwapchainFormat);

  [[ nodiscard ]] VkSwapchainKHR vkSwapchain() {
    return this->vkSwapchain_;
  }

  [[ nodiscard ]] VkSurfaceFormatKHR vkSwapchainFormat() {
    return this->vkSwapchainFormat_;
  }

  [[ nodiscard ]] std::vector<std::shared_ptr<SwapchainImage>>& images() {
    return this->images_;
  }
};

}
