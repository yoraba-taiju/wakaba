/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "FrameBuffer.hpp"
#include "Vulkan.hpp"

namespace vk {

vk::FrameBuffer::~FrameBuffer() noexcept{
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyFramebuffer(vulkan->vkDevice(), vkFramebuffer_, nullptr);
  }
}

}