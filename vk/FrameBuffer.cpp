//
// Created by kaede on 2019/11/13.
//

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