/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include <memory>
#include <vector>

#include "../util/File.hpp"
#include "../util/Logger.hpp"

#include "Vulkan.hpp"
#include "RenderPass.hpp"
#include "Framebuffer.hpp"
#include "ShaderModule.hpp"
#include "builder/GraphicsPipelineBuilder.hpp"
#include "command/CommandBuffer.hpp"
#include "command/CommandPool.hpp"
#include "Util.hpp"

namespace vk {

Vulkan::Vulkan(util::Logger &log)
:log_(log)
{
}

Vulkan::~Vulkan() {
  vkDestroySurfaceKHR(this->vkInstance_, this->vkSurface_, nullptr);
  this->vkDestroyDebugReportCallback_(this->vkInstance_, this->vkDebugReportCallback_, nullptr);
  vkDestroyInstance(this->vkInstance_, nullptr);
  glfwDestroyWindow(this->window_);
}

std::shared_ptr<Vulkan> Vulkan::create(util::Logger& log) {
  return std::make_shared<Vulkan>(log);
}

std::shared_ptr<Device> Vulkan::createDevice() {
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  {
    static float qPriorities[] = {1.0f};
    // Graphics queue
    queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = vkGraphicsQueueFamilyIndex_,
        .queueCount = 1,
        .pQueuePriorities = qPriorities,
    });
    if(vkGraphicsQueueFamilyIndex_ != vkPresentQueueFamilyIndex_) {
      // Present queue
      queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
          .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
          .pNext = nullptr,
          .flags = 0,
          .queueFamilyIndex = vkPresentQueueFamilyIndex_,
          .queueCount = 1,
          .pQueuePriorities = qPriorities,
      });
    }
  }
  const char *extensions[] = {"VK_KHR_swapchain"};
  VkDeviceCreateInfo devInfo = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
      .pQueueCreateInfos = queueCreateInfos.data(),
      // enabledLayerCount is deprecated and ignored.
      // ppEnabledLayerNames is deprecated and ignored.
      // See https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#extendingvulkan-layers-devicelayerdeprecation.
      .enabledLayerCount = 0,
      .ppEnabledLayerNames = nullptr,
      .enabledExtensionCount = std::size(extensions),
      .ppEnabledExtensionNames = extensions,
      .pEnabledFeatures = nullptr, // optional
  };

  VkDevice device;
  if (vkCreateDevice(vkPhysicalDevice_, &devInfo, nullptr, &device) != VK_SUCCESS) {
    log().fatal("[Vulkan] Failed to create a device.");
  }
  VkQueue vkGraphicsQueue;
  VkQueue vkPresentQueue;
  vkGetDeviceQueue(device, vkGraphicsQueueFamilyIndex_, 0, &vkGraphicsQueue);
  vkGetDeviceQueue(device, vkPresentQueueFamilyIndex_, 0, &vkPresentQueue);
  return std::make_shared<Device>(shared_from_this(), device, vkGraphicsQueue, vkPresentQueue);
}

}