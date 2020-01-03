/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once
#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vk {

class Device;

class Pipeline {
private:
  std::shared_ptr<Device> device_;
  VkPipeline vkPipeline_;
public:
  Pipeline() = delete;
  Pipeline(Pipeline const&) = delete;
  Pipeline& operator=(Pipeline const&) = delete;

protected:
  Pipeline(Pipeline&&) = default;
  Pipeline& operator=(Pipeline&&) = default;

  explicit Pipeline(std::shared_ptr<Device> device, VkPipeline pipeline);
  ~Pipeline() noexcept;

public:
  [[ nodiscard ]] VkPipeline vkPipeline() {
    return this->vkPipeline_;
  }

};

}
