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

class Vulkan;
class Buffer;

class BufferBuilder {
private:
  std::shared_ptr<Vulkan> vulkan_;
  VkBufferCreateInfo vkBufferCreateInfo_{};
  VkDeviceSize size_{};
  VkBufferUsageFlags usages_{};
public:
  BufferBuilder(BufferBuilder&&) = delete;
  BufferBuilder(BufferBuilder const&) = delete;
  BufferBuilder& operator=(BufferBuilder&&) = delete;
  BufferBuilder& operator=(BufferBuilder const&) = delete;
  BufferBuilder() = delete;

  explicit BufferBuilder(std::shared_ptr<Vulkan> vulkan, VkDeviceSize size);
  BufferBuilder& addUsages(VkBufferUsageFlags usage);
  BufferBuilder& setUsages(VkBufferUsageFlags usages);
  BufferBuilder& clearUsages();
  Buffer build();
};

}


