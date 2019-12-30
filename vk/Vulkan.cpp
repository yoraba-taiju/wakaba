//
// Created by kaede on 2019/10/29.
//

#include <memory>
#include <vector>

#include "../util/File.hpp"

#include "Vulkan.hpp"
#include "RenderPass.hpp"
#include "FrameBuffer.hpp"
#include "ShaderModule.hpp"
#include "GraphicsPipelineBuilder.hpp"

namespace vk {

//Builderみたいなのほしい

Vulkan::Vulkan(util::Logger &log)
: log_(log)
{
}

Vulkan::~Vulkan() {
}

void Vulkan::destroy() {
  frameBuffers_.clear();
  swapchainImages_.clear();
  vkDestroySwapchainKHR(vkDevice_, vkSwapchain_, nullptr);

  vkDestroyFence(vkDevice_, fence_, nullptr);

  vkDestroyDevice(vkDevice_, nullptr);
  vkDestroySurfaceKHR(this->vkInstance_, this->vkSurface_, nullptr);
  this->vkDestroyDebugReportCallback_(this->vkInstance_, this->vkDebugReportCallback_, nullptr);

  vkDestroyInstance(this->vkInstance_, nullptr);
  glfwDestroyWindow(this->window_);
}


std::shared_ptr<GraphicsPipelineBuilder> Vulkan::createGraphicsPipelineBuilder() {
  return util::make_shared<GraphicsPipelineBuilder>(self());
}

}