/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Device.hpp"
#include "PipelineLayout.hpp"

namespace vk {

PipelineLayout::PipelineLayout(std::shared_ptr<Device> device, VkPipelineLayout pipelineLayout)
:device_(std::move(device))
,vkPipelineLayout_(pipelineLayout) {
}

PipelineLayout::~PipelineLayout() noexcept {
  if(device_) {
    device_->destroyPipelineLayout(*this);
  }
}

}
