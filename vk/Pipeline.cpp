/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Device.hpp"
#include "Pipeline.hpp"

namespace vk {

Pipeline::Pipeline(std::shared_ptr<Device> device, VkPipeline pipeline)
:device_(std::move(device))
,vkPipeline_(pipeline)
{

}

Pipeline::~Pipeline() noexcept {
  if(device_) {
    device_->destroyPipeline(*this);
  }
}


}
