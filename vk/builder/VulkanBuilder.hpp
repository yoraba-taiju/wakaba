/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>
#include <string>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "../../util/Logger.hpp"
#include "../FrameBuffer.hpp"
#include "../Vulkan.hpp"

namespace vk {
class Vulkan;

class VulkanBuilder final {
private:
  util::Logger& log_;
  std::shared_ptr<Vulkan> vulkan_;
  std::string appName_;
  int const width_;
  int const height_;
private:
  [[ nodiscard ]] util::Logger& log() { return this->log_; }
  [[ nodiscard ]] std::shared_ptr<Vulkan> const& vulkan() { return this->vulkan_; }
private:
  void createWindow();
  void createInstance();
  void createSurface();
  void createDebugReportCallback();
  void createDeviceAndCommandPool();
  void createFence();
  void createSwapchain();
  void createSwapchainImages();
  void createFrameBuffers();

public:
  explicit VulkanBuilder(util::Logger &log, std::string appName, int width, int height);
  VulkanBuilder() = delete;
  VulkanBuilder(VulkanBuilder&&) = delete;
  VulkanBuilder(VulkanBuilder const&) = delete;
  VulkanBuilder& operator=(VulkanBuilder&&) = delete;
  VulkanBuilder& operator=(VulkanBuilder const&) = delete;

public:
  std::shared_ptr<Vulkan> build();
};


}
