/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include <utility>

#include "Vulkan.hpp"
#include "Device.hpp"
#include "Framebuffer.hpp"
#include "image/SwapchainImage.hpp"
#include "RenderPass.hpp"
#include "Swapchain.hpp"
#include "Pipeline.hpp"
#include "PipelineLayout.hpp"
#include "buffer/Buffer.hpp"
#include "buffer/DeviceMemory.hpp"
#include "command/CommandPool.hpp"
#include "command/CommandBuffer.hpp"
#include "Util.hpp"

namespace vk {

Device::Device(std::shared_ptr<Vulkan> vulkan, VkDevice device, VkQueue vkGraphicsQueue, VkQueue vkPresentQueue)
:vulkan_(std::move(vulkan))
,vkDevice_(device)
,vkGraphicsQueue_(vkGraphicsQueue)
,vkPresentQueue_(vkPresentQueue)
{
}

Device::~Device() noexcept {
  vkDestroyDevice(vkDevice_, nullptr);
}

uint32_t Device::vkGraphicsQueueFamilyIndex() {
  return this->vulkan_->vkGraphicsQueueFamilyIndex();
}

uint32_t Device::vkPresentQueueFamilyIndex() {
  return this->vulkan_->vkPresentQueueFamilyIndex();
}

std::shared_ptr<Swapchain> Device::createSwapchain() {
  VkSurfaceCapabilitiesKHR surfaceCaps{};
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan_->vkPhysicalDevice(), vulkan_->vkSurface(), &surfaceCaps);

  auto surfaceFormats = getPhysicalDeviceSurfaceFormats(vulkan_->vkPhysicalDevice(), vulkan_->vkSurface());
  std::optional<size_t> formatIdx;
  for (size_t i = 0; i < surfaceFormats.size(); ++i) {
    auto format = surfaceFormats[i];
    if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
      formatIdx = i;
      break;
    }
  }
  if (!formatIdx.has_value()) {
    throw std::runtime_error("[Vulkan] VK_FORMAT_B8G8R8A8_UNORM & VK_COLORSPACE_SRGB_NONLINEAR_KHR is not supported.");
  }

  VkSurfaceFormatKHR const vkSwapchainFormat = surfaceFormats[formatIdx.value()];

  auto presentModes = getPhysicalDeviceSurfacePresentModes(vulkan_->vkPhysicalDevice(), vulkan_->vkSurface());
  std::optional<size_t> presentModeIdx;
  for (size_t i = 0; i < presentModes.size(); ++i) {
    auto mode = presentModes[i];
    if (mode == VK_PRESENT_MODE_FIFO_KHR) {
      presentModeIdx = i;
    }
  }

  if (!presentModeIdx.has_value()) {
    throw std::runtime_error("VK_PRESENT_MODE_FIFO_KHR is not supported.");
  }

  VkSwapchainCreateInfoKHR swapchainCreateInfo{
      .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
      .pNext= nullptr,
      .flags = 0,
      .surface = vulkan_->vkSurface(),
      .minImageCount = surfaceCaps.minImageCount,
      .imageFormat = surfaceFormats[formatIdx.value()].format, // VK_FORMAT_B8G8R8A8_UNORM
      .imageColorSpace = surfaceFormats[formatIdx.value()].colorSpace, // VK_COLORSPACE_SRGB_NONLINEAR_KHR
      .imageExtent = {
        .width = vulkan_->width(),
        .height = vulkan_->height(),
      },
      .imageArrayLayers = 1,
      .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
      /*
       * ここの間は下で埋める
       */
      .preTransform = surfaceCaps.currentTransform,
      .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
      .presentMode = presentModes[presentModeIdx.value()], //VK_PRESENT_MODE_FIFO_KHR
      .clipped = VK_TRUE,
      .oldSwapchain = VK_NULL_HANDLE,
  };
  std::vector<uint32_t> queueFamilyIndex;
  if (vulkan_->vkGraphicsQueueFamilyIndex() != vulkan_->vkPresentQueueFamilyIndex()) {
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    queueFamilyIndex.emplace_back(vulkan_->vkGraphicsQueueFamilyIndex());
    queueFamilyIndex.emplace_back(vulkan_->vkPresentQueueFamilyIndex());
    swapchainCreateInfo.queueFamilyIndexCount = queueFamilyIndex.size();
    swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndex.data();
  } else {
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.queueFamilyIndexCount = 0; // Optional
    swapchainCreateInfo.pQueueFamilyIndices = nullptr; // Optional
  }

  VkSwapchainKHR vkSwapchain;
  if (vkCreateSwapchainKHR(vkDevice_, &swapchainCreateInfo, nullptr, &vkSwapchain)) {
    throw std::runtime_error("Failed to create swap chain.");
  }

  std::vector<VkImage> vkImages = getSwapchainImages(vkDevice_, vkSwapchain);
  if (vkImages.empty()) {
    throw std::runtime_error("No swapchain images available.");
  }
  std::vector<std::shared_ptr<SwapchainImage>> images;
  for(VkImage& vkImage : vkImages) {
    VkImageViewCreateInfo imageViewCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .image = vkImage,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = vkSwapchainFormat.format,
        .components = {
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
        },
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        }
    };
    VkImageView vkImageView;
    if (vkCreateImageView(vkDevice_, &imageViewCreateInfo, nullptr, &vkImageView) != VK_SUCCESS) {
      throw std::runtime_error("[Vulkan] Failed to create an image view");
    }
    images.emplace_back(std::make_shared<SwapchainImage>(shared_from_this(), vkImage, vkImageView, vulkan_->width(), vulkan_->height()));
  }
  return std::make_shared<Swapchain>(shared_from_this(), vkSwapchain, vkSwapchainFormat, std::move(images));
}

std::shared_ptr<CommandPool> Device::createCommandPool() {
  VkCommandPool vkCommandPool;
  {
    VkCommandPoolCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = vkGraphicsQueueFamilyIndex(),
    };
    if (vkCreateCommandPool(vkDevice_, &info, nullptr, &vkCommandPool) != VK_SUCCESS) {
      throw std::runtime_error("[Device] Failed to create a command pool.");
    }
  }
  return std::make_shared<CommandPool>(shared_from_this(), vkGraphicsQueue(), vkCommandPool);
}

void Device::destroyFramebuffer(Framebuffer& framebuffer) {
  vkDestroyFramebuffer(vkDevice_, framebuffer.vkFramebuffer(), nullptr);
}

void Device::destroyRenderPass(RenderPass& renderPass) {
  vkDestroyRenderPass(vkDevice_, renderPass.vkRenderPass(), nullptr);
}

void Device::destroyShaderModule(ShaderModule& shaderModule) {
  vkDestroyShaderModule(vkDevice_, shaderModule.vkShaderModule(), nullptr);
}

void Device::destroyCommandPool(CommandPool& commandPool) {
  vkDestroyCommandPool(vkDevice_, commandPool.vkCommandPool(), nullptr);
}

void Device::destroyPipelineLayout(PipelineLayout& pipelineLayout) {
  vkDestroyPipelineLayout(vkDevice_, pipelineLayout.vkPipelineLayout(), nullptr);
}

void Device::destroySwapchain(Swapchain& swapchain) {
  vkDestroySwapchainKHR(vkDevice_, swapchain.vkSwapchain(), nullptr);
}

void Device::destroyPipeline(Pipeline& pipeline) {
  vkDestroyPipeline(vkDevice_, pipeline.vkPipeline(), nullptr);
}

void Device::destroyBuffer(Buffer& buffer) {
  vkDestroyBuffer(vkDevice_, buffer.vkBuffer(), nullptr);
}

void Device::destroyDeviceMemory(DeviceMemory& deviceMemory) {
  vkFreeMemory(vkDevice_, deviceMemory.vkDeviceMemory(), nullptr);
}

void Device::destroyCommandBuffer(CommandBuffer& commandBuffer) {
  VkCommandBuffer vkCommandBuffer = commandBuffer.vkCommandBuffer();
  vkFreeCommandBuffers(vkDevice_, commandBuffer.commandPool()->vkCommandPool(), 1, &vkCommandBuffer);
}

}
