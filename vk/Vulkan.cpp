//
// Created by kaede on 2019/10/29.
//

#include <memory>
#include <vector>

#include "Vulkan.hpp"

namespace vk {

//Builderみたいなのほしい

Vulkan::Vulkan(util::Logger& log)
:log_(log)
,instance_()
,surface_()
,window_()
,device_()
,commandPool_()
,fence_()
{
}

Vulkan::~Vulkan() {
  vkDestroyCommandPool(this->device_, this->commandPool_, nullptr);
  vkDestroyDevice(this->device_, nullptr);
  vkDestroySurfaceKHR(this->instance_, this->surface_, nullptr);
  this->vkDestroyDebugReportCallback_(this->instance_, this->vkDebugReportCallback_, nullptr);
  vkDestroyInstance(this->instance_, nullptr);
  glfwDestroyWindow(this->window_);
}

}