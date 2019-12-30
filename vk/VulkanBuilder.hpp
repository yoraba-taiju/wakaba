//
// Created by kaede on 2019/10/30.
//

#pragma once

#include <memory>
#include <string>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "../util/Logger.hpp"
#include "FrameBuffer.hpp"
#include "Vulkan.hpp"

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
  VulkanBuilder(util::Logger &log, std::string appName, int width, int height);

  std::shared_ptr<Vulkan> create();

};


}
