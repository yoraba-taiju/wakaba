/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "DeviceMemory.hpp"
#include "../Vulkan.hpp"

namespace vk{

DeviceMemory::DeviceMemory(std::shared_ptr<Vulkan> const& vulkan, VkDeviceMemory vkDeviceMemory, VkDeviceSize size)
:vulkan_(vulkan)
,vkDeviceMemory_(vkDeviceMemory)
,size_(size)
{

}

DeviceMemory::~DeviceMemory() noexcept {
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkFreeMemory(vulkan->vkDevice(), this->vkDeviceMemory_, nullptr);
  }
}

}
