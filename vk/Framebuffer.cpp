/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Framebuffer.hpp"
#include "Vulkan.hpp"

namespace vk {

vk::Framebuffer::~Framebuffer() noexcept{
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyFramebuffer(vulkan->vkDevice(), vkFramebuffer_, nullptr);
  }
}

}