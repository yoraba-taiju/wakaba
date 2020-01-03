/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "DeviceMemoryBuilder.hpp"

#include <utility>
#include "../Vulkan.hpp"
#include "../buffer/DeviceMemory.hpp"

namespace vk {

DeviceMemoryBuilder::DeviceMemoryBuilder(std::shared_ptr<Device> device, VkMemoryRequirements const& requirements, VkMemoryPropertyFlags propertyFlags)
:device_(std::move(device))
,requirements_(requirements)
,propertyFlags_(propertyFlags)
{

}

std::shared_ptr<DeviceMemory> DeviceMemoryBuilder::build() {
  VkPhysicalDeviceMemoryProperties const& memProperties = device_->vulkan()->vkPhysicalDeviceMemoryProperties();

  std::optional<uint32_t> memoryType;
  {
    uint32_t const memoryTypeBits = this->requirements_.memoryTypeBits;
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
      if ((memoryTypeBits & (1u << i)) == (1u << i) && (memProperties.memoryTypes[i].propertyFlags & propertyFlags_) == propertyFlags_) {
        memoryType = i;
        break;
      }
    }
  }
  if(!memoryType.has_value()) {
    throw std::runtime_error(fmt::format("Failed to find suitable memory type: flags={:08x}", propertyFlags_));
  }
  VkMemoryAllocateInfo allocInfo = {
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .pNext = nullptr,
      .allocationSize = this->requirements_.size,
      .memoryTypeIndex = memoryType.value(),
  };
  VkDeviceMemory vkDeviceMemory;
  if (vkAllocateMemory(device_->vkDevice(), &allocInfo, nullptr, &vkDeviceMemory) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate image memory!");
  }
  return std::make_shared<DeviceMemory>(device_, vkDeviceMemory, requirements_.size);
}

}
