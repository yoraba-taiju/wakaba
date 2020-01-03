/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Framebuffer.hpp"
#include "Vulkan.hpp"

namespace vk {

vk::Framebuffer::~Framebuffer() noexcept {
  if(device_) {
    device_->destroyFramebuffer(*this);
  }
}

}