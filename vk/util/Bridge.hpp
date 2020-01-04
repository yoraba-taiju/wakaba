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

#include "../buffer/Buffer.hpp"

namespace vk{
class Device;
class CommandPool;
class Buffer;

class Bridge final {
private:
  static constexpr VkDeviceSize StagingBufferSize = 1024 * 1024 * 16; //16MB
private:
  std::shared_ptr<Device> device_;
  std::unordered_map<std::thread::id, std::shared_ptr<CommandPool>> commandPools_{};
  std::unique_ptr<std::mutex> commandPoolMutex_{};
  std::unordered_map<std::thread::id, std::unique_ptr<Buffer>> stagingBuffer_{};
  std::unique_ptr<std::mutex> stagingBufferMutex_{};
private:
  std::shared_ptr<CommandPool> commandPool();
  Buffer& stagingBuffer();
public:
  Bridge() = delete;
  Bridge(Bridge const&) = delete;
  Bridge(Bridge&&) = default;
  Bridge& operator=(Bridge const&) = delete;
  Bridge& operator=(Bridge&&) = default;
  explicit Bridge(std::shared_ptr<Device> device);
  ~Bridge() noexcept = default;
public:
  void updateBuffer(Buffer& buffer, VkDeviceSize offset, void const* src, size_t size);
};

}
