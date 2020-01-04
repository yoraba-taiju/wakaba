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
#include <thread>
#include <unordered_map>
#include <mutex>

namespace vk{
class Device;
class CommandPool;

class Bridge {
private:
  std::shared_ptr<Device> device_;
  std::unordered_map<std::thread::id, std::shared_ptr<CommandPool>> commandPools_;
  std::mutex mutex_;
public:

};

}
