//
// Created by kaede on 2019/10/29.
//

#include <memory>
#include <vector>

#include "Vulkan.hpp"
#include "RenderPass.hpp"
#include "FrameBuffer.hpp"
#include "ShaderModule.hpp"
#include "../util/File.hpp"

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

  VkShaderModule mod;
  std::shared_ptr<ShaderModule> shader = util::make_shared<ShaderModule>();
  if (vkCreateShaderModule(device_, &shaderInfo, nullptr, &shader->obj_) != VK_SUCCESS) {
    log_.fatal("Failed to create shader module: %s", filename);
  }
  return std::move(shader);
}

}