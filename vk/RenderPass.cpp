//
// Created by kaede on 2019/11/13.
//

#include "RenderPass.hpp"
#include "Vulkan.hpp"

namespace vk {

RenderPass::~RenderPass() noexcept{
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
    vkDestroyRenderPass(vulkan->device(), vkRenderPass_, nullptr);
  }
}

}
