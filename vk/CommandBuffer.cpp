//
// Created by kaede on 2019/11/13.
//

#include "CommandBuffer.hpp"
#include "Vulkan.hpp"

namespace vk {

CommandBuffer::~CommandBuffer() noexcept{
  std::shared_ptr<Vulkan> vulkan =  vulkan_.lock();
  if(vulkan) {
  }
}

}