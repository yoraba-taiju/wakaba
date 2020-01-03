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
#include <vector>
#include <optional>

namespace vk {

class Device;
class Buffer;
class DeviceMemory;

class BufferBuilder {
private:
  std::shared_ptr<Device> device_;
  VkBufferCreateInfo vkBufferCreateInfo_{};
private:
  VkDeviceSize size_{};
  VkBufferUsageFlags usages_{};
public:
  BufferBuilder(BufferBuilder&&) = delete;
  BufferBuilder(BufferBuilder const&) = delete;
  BufferBuilder& operator=(BufferBuilder&&) = delete;
  BufferBuilder& operator=(BufferBuilder const&) = delete;
  BufferBuilder() = delete;

  explicit BufferBuilder(std::shared_ptr<Device> device, VkDeviceSize size);
  BufferBuilder& addUsages(VkBufferUsageFlags usage);
  BufferBuilder& setUsages(VkBufferUsageFlags usages);
  BufferBuilder& clearUsages();
  Buffer build();
};

}


