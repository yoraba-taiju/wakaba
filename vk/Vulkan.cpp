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
, instance_()
, surface_()
, window_()
, physicalDevice_()
, queueFamilyIndex_()
, device_()
, fence_()
, vkDebugReportCallback_()
, vkDestroyDebugReportCallback_()
, swapchain_()
, swapchainImages_()
, swapchainImageViews_()
, frameBuffers_()
{
}

Vulkan::~Vulkan() {
}

void Vulkan::destroy() {
  frameBuffers_.clear();
  for(auto & view : swapchainImageViews_) {
    vkDestroyImageView(device_, view, nullptr);
  }
  vkDestroySwapchainKHR(device_, swapchain_, nullptr);

  vkDestroyFence(device_, fence_, nullptr);

  vkDestroyDevice(device_, nullptr);
  vkDestroySurfaceKHR(this->instance_, this->surface_, nullptr);
  this->vkDestroyDebugReportCallback_(this->instance_, this->vkDebugReportCallback_, nullptr);

  vkDestroyInstance(this->instance_, nullptr);
  glfwDestroyWindow(this->window_);
}

std::shared_ptr<ShaderModule> Vulkan::loadShaderFromFile(std::string const& filename) {
  std::vector<uint8_t> data = util::readAllFromFile(filename);
  VkShaderModuleCreateInfo shaderInfo{};

  shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  shaderInfo.codeSize = data.size();
  shaderInfo.pCode = reinterpret_cast<uint32_t*>(data.data());

  VkShaderModule mod{};
  if (vkCreateShaderModule(device_, &shaderInfo, nullptr, &mod) != VK_SUCCESS) {
    log_.fatal("Failed to create shader module: %s", filename);
  }
  std::shared_ptr<ShaderModule> shader = util::make_shared<ShaderModule>(self(), mod);
  return std::move(shader);
}

std::shared_ptr<GraphicsPipelineBuilder> Vulkan::createGraphicsPipelineBuilder() {
  return util::make_shared<GraphicsPipelineBuilder>(self());
}

}