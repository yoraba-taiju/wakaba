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

DeviceMemoryBuilder::DeviceMemoryBuilder(std::shared_ptr<Vulkan> vulkan,VkDeviceSize const allocationSize,
                                         VkMemoryRequirements const& requirements, VkMemoryPropertyFlags propertyFlags)
    :vulkan_(std::move(vulkan))
    ,allocationSize_(allocationSize)
    ,requirements_(requirements)
    ,propertyFlags_(propertyFlags)
{

}

std::shared_ptr<DeviceMemory> DeviceMemoryBuilder::build() {
  VkPhysicalDeviceMemoryProperties const& memProperties = vulkan_->vkPhysicalDeviceMemoryProperties();

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
    vulkan_->log().fatal("Failed to find suitable memory type: flags={:08x}", propertyFlags_);
  }
  VkMemoryAllocateInfo allocInfo = {
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .pNext = nullptr,
      .allocationSize = allocationSize_,
      .memoryTypeIndex = memoryType.value(),
  };
  VkDeviceMemory vkDeviceMemory;
  if (vkAllocateMemory(vulkan_->vkDevice(), &allocInfo, nullptr, &vkDeviceMemory) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate image memory!");
  }
  return std::make_shared<DeviceMemory>(vulkan_, vkDeviceMemory, allocationSize_);
}

}
