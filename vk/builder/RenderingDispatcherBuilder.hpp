/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>
#include "../RenderingDispatcher.hpp"


namespace vk {

class Vulkan;
class RenderingDispatcher;

class RenderingDispatcherBuilder final {
private:
  std::shared_ptr<Vulkan> vulkan_;

public:
  explicit RenderingDispatcherBuilder(std::shared_ptr<Vulkan> vulkan);
  RenderingDispatcherBuilder(RenderingDispatcherBuilder&&) = delete;
  RenderingDispatcherBuilder(RenderingDispatcherBuilder const&) = delete;
  RenderingDispatcherBuilder& operator=(RenderingDispatcherBuilder&&) = delete;
  RenderingDispatcherBuilder& operator=(RenderingDispatcherBuilder const&) = delete;

public:
  RenderingDispatcher build();
};

}
