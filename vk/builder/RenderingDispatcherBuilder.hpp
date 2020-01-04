/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>
#include "../helper/RenderingDispatcher.hpp"

namespace vk {

class Device;
class RenderingDispatcher;

class RenderingDispatcherBuilder final {
private:
  std::shared_ptr<Device> device_;
  std::shared_ptr<Swapchain> swapchain_;
public:
  explicit RenderingDispatcherBuilder(std::shared_ptr<Device> device, std::shared_ptr<Swapchain> swapchain);
  RenderingDispatcherBuilder(RenderingDispatcherBuilder&&) = delete;
  RenderingDispatcherBuilder(RenderingDispatcherBuilder const&) = delete;
  RenderingDispatcherBuilder& operator=(RenderingDispatcherBuilder&&) = delete;
  RenderingDispatcherBuilder& operator=(RenderingDispatcherBuilder const&) = delete;

public:
  RenderingDispatcher build();
};

}
