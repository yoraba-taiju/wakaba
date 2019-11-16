//
// Created by kaede on 2019/10/30.
//

#include <utility>
#include <vector>
#include <unordered_set>
#include "../util/Shared.hpp"
#include "Util.hpp"
#include "VulkanBuilder.hpp"
#include "Vulkan.hpp"
#include "RenderPass.hpp"
#include "FrameBuffer.hpp"
#include "CommandBuffer.hpp"

namespace vk {

namespace {

VKAPI_ATTR VkBool32 VKAPI_CALL onError(
  VkDebugReportFlagsEXT flags,
  VkDebugReportObjectTypeEXT,
  uint64_t object,
  size_t location,
  int32_t messageCode,
  const char *pLayerPrefix,
  const char *pMessage,
  void *pUserData)
{
  auto vk = reinterpret_cast<Vulkan *>(pUserData);
  vk->log().debug("[From Vulkan :: %s] \n%s", pLayerPrefix, pMessage);
  return VK_FALSE;
}

}

VulkanBuilder::VulkanBuilder(util::Logger &log, std::string appName, int width, int height)
    : log_(log), appName_(std::move(appName)), width_(width), height_(height), vulkan_() {
}

std::shared_ptr<Vulkan> VulkanBuilder::create() {
  this->vulkan_ = std::move(util::make_shared<Vulkan>(this->log_));

  this->createWindow();
  this->createInstance();
  this->createSurface();
  this->createDebugReportCallback();
  this->createDeviceAndCommandPool();
  this->createFence();
  this->createSwapchain();
  this->createSwapchainImages();
  this->createSwapchainImageViews();
  this->createFrameBuffers();
  this->createCommandBuffers();

  return this->vulkan_;
}

// Open window
void VulkanBuilder::createWindow() {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  this->vulkan_->window_ = glfwCreateWindow(width_, height_, "YorabaTaiju", nullptr, nullptr);

  if (this->vulkan_->window_ == nullptr) {
    glfwTerminate();
    log_.fatal("Failed to open GLFW window.");
  }
}

// Initialize Vulkan and create Instance.
void VulkanBuilder::createInstance() {
  VkInstanceCreateInfo instanceInfo{};
  VkApplicationInfo appInfo{};

  std::vector<std::string> const requiredExtensions = enumerateRequiredInstanceExtensions();
  uint32_t numExtensions = requiredExtensions.size() + 1;
  const char *extensionNames[numExtensions];
  log_.debug("[[Required Vulkan Extensions]]");
  for (size_t i = 0; i < requiredExtensions.size(); ++i) {
    extensionNames[i] = requiredExtensions[i].c_str();
    log_.debug(" - %s", extensionNames[i]);
  }
  extensionNames[numExtensions - 1] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;

  std::vector<VkLayerProperties> layers = vk::enumerateInstanceLayerProperties();
  std::vector<const char*> enabledLayers{};
  // FIXME: どこにおく？ここでいいか？
  std::unordered_set<std::string> desiredLayers{
    "VK_LAYER_LUNARG_monitor",
     "VK_LAYER_LUNARG_standard_validation",
     "VK_LAYER_KHRONOS_validation",
     "VK_LAYER_LUNARG_screenshot",
  };
  log_.debug("[[Available Vulkan Layers]]");
  for (auto & layer : layers) {
    log_.debug(" - %s (%s :: spec=%d, impl=%d)",
               layer.layerName,
               layer.description,
               layer.specVersion,
               layer.implementationVersion);
    if(desiredLayers.find(layer.layerName) != desiredLayers.end()) {
      enabledLayers.emplace_back(layer.layerName);
      log_.debug("   - enabled!");
    }
  }

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 1, 126);
  appInfo.pApplicationName = this->appName_.c_str();
  appInfo.apiVersion = VK_API_VERSION_1_1;

  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.enabledExtensionCount = requiredExtensions.size() + 1;
  instanceInfo.ppEnabledExtensionNames = extensionNames;
  instanceInfo.enabledLayerCount = enabledLayers.size();
  instanceInfo.ppEnabledLayerNames = enabledLayers.data();

  if (vkCreateInstance(&instanceInfo, nullptr, &this->vulkan_->instance_) != VK_SUCCESS) {
    log_.fatal("Failed to create Vulkan Instance");
  }
}

// Attach the instance to the window.
void VulkanBuilder::createSurface() {
  VkResult const res = glfwCreateWindowSurface(
      this->vulkan_->instance_,
      this->vulkan_->window_,
      nullptr,
      &this->vulkan_->surface_);
  if (res != VK_SUCCESS) {
    log_.fatal("Failed to create Vulkan Surface");
  }
}

// Create vulkan's debug reporter callback.
void VulkanBuilder::createDebugReportCallback() {
  VkDebugReportCallbackCreateInfoEXT callbackInfo{};
  callbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  callbackInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
                       VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;// | VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
  callbackInfo.pfnCallback = &onError;
  callbackInfo.pUserData = vulkan_.get();

  auto _vkCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT) (glfwGetInstanceProcAddress(
      this->vulkan_->instance_, "vkCreateDebugReportCallbackEXT"));
  if (_vkCreateDebugReportCallback == nullptr) {
    log_.fatal("[Vulkan] vkCreateDebugReportCallbackEXT not available.");
  }
  auto _vkDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT) (glfwGetInstanceProcAddress(
      this->vulkan_->instance_, "vkDestroyDebugReportCallbackEXT"));
  if (_vkDestroyDebugReportCallback == nullptr) {
    log_.fatal("[Vulkan] vkDestroyDebugReportCallbackEXT not available.");
  }
  if (_vkCreateDebugReportCallback(vulkan_->instance_, &callbackInfo, nullptr, &vulkan_->vkDebugReportCallback_) !=
      VK_SUCCESS) {
    log_.fatal("Failed initialize a debug reporter.");
  }
  vulkan_->vkDestroyDebugReportCallback_ = _vkDestroyDebugReportCallback;
}

void VulkanBuilder::createDeviceAndCommandPool() {
  std::vector<VkPhysicalDevice> physicalDevices = enumeratePhysicalDevices(vulkan_->instance_);
  if (physicalDevices.empty()) {
    log_.fatal("[Vulkan] No vulkan physical devices available.");
  }
  log_.debug("[[Available Vulkan Physical Devices]]");
  for (VkPhysicalDevice device : physicalDevices) {
    VkPhysicalDeviceProperties props;
    VkPhysicalDeviceMemoryProperties memProps;
    vkGetPhysicalDeviceProperties(device, &props);
    vkGetPhysicalDeviceMemoryProperties(device, &memProps);
    log_.info(" - %s", props.deviceName);
    log_.info("   - API version: %08x", props.apiVersion);
    log_.info("   - driver version: %08x", props.driverVersion);
    log_.info("   - Vendor ID: %08x", props.vendorID);
    log_.info("   - Device ID: %08x", props.deviceID);
  }
  {
    VkDeviceCreateInfo devInfo{};
    VkDeviceQueueCreateInfo queueInfo{};
    vulkan_->physicalDevice_ = physicalDevices[0];

    { // Search queue family index for Graphics Queue
      uint32_t queueFamilyIndex = 0xffffffff;
      std::vector<VkQueueFamilyProperties> properties = getPhysicalDeviceQueueFamilyProperties(
          vulkan_->physicalDevice_);
      for (size_t i = 0; i < properties.size(); ++i) {
        if ((properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
          queueFamilyIndex = i;
          break;
        }
      }
      if (queueFamilyIndex == 0xffffffff) {
        log_.fatal("No Graphics queues available.");
      }
      vulkan_->queueFamilyIndex_ = queueFamilyIndex;
    }

    const char *layers[] = {"VK_LAYER_LUNARG_standard_validation"};
    const char *extensions[] = {"VK_KHR_swapchain"};
    static float qPriorities[] = {0.0f};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueCount = 1;
    queueInfo.queueFamilyIndex = vulkan_->queueFamilyIndex_;
    queueInfo.pQueuePriorities = qPriorities;
    devInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    devInfo.queueCreateInfoCount = 1;
    devInfo.pQueueCreateInfos = &queueInfo;
    devInfo.enabledLayerCount = std::size(layers);
    devInfo.ppEnabledLayerNames = layers;
    devInfo.enabledExtensionCount = std::size(extensions);
    devInfo.ppEnabledExtensionNames = extensions;

    if (vkCreateDevice(vulkan_->physicalDevice_, &devInfo, nullptr, &vulkan_->device_) != VK_SUCCESS) {
      log_.fatal("[Vulkan] Failed to create a device.");
    }

    VkCommandPoolCreateInfo info{};

    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.queueFamilyIndex = vulkan_->queueFamilyIndex_;
    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(vulkan_->device_, &info, nullptr, &vulkan_->commandPool_) != VK_SUCCESS) {
      log_.fatal("[Vulkan] Failed to create a command pool.");
    }
  }
}

void VulkanBuilder::createFence() {
  VkFenceCreateInfo finfo{};
  finfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  if (vkCreateFence(vulkan_->device_, &finfo, nullptr, &vulkan_->fence_) != VK_SUCCESS) {
    log_.fatal("[Vulkan] Failed to create a fence.");
  }
}

void VulkanBuilder::createSwapchain() {
  VkSwapchainCreateInfoKHR scinfo{};

  VkBool32 surfaceSupported;
  vkGetPhysicalDeviceSurfaceSupportKHR(vulkan_->physicalDevice_, vulkan_->queueFamilyIndex_, vulkan_->surface_,
                                       &surfaceSupported);
  if (surfaceSupported != VK_TRUE) {
    log_.fatal("[Vulkan] Physical device does not support surface.");
  }

  VkSurfaceCapabilitiesKHR surfaceCaps;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan_->physicalDevice_, vulkan_->surface_, &surfaceCaps);
  // TODO: キャパシティのチェック

  auto surfaceFormats = getPhysicalDeviceSurfaceFormats(vulkan_->physicalDevice_, vulkan_->surface_);
  auto presentModes = getPhysicalDeviceSurfacePresentModes(vulkan_->physicalDevice_, vulkan_->surface_);

  size_t formatIdx = 0xffffffff;
  for (size_t i = 0; i < surfaceFormats.size(); ++i) {
    auto format = surfaceFormats[i];
    if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
      formatIdx = i;
    }
  }
  if (formatIdx == 0xffffffff) {
    log_.fatal("[Vulkan] VK_FORMAT_B8G8R8A8_UNORM & VK_COLORSPACE_SRGB_NONLINEAR_KHR is not supported.");
  }

  size_t presentModeIdx = 0xffffffff;
  for (size_t i = 0; i < presentModes.size(); ++i) {
    auto mode = presentModes[i];
    if (mode == VK_PRESENT_MODE_FIFO_KHR) {
      presentModeIdx = i;
    }
  }
  if (presentModeIdx == 0xffffffff) {
    log_.fatal("[Vulkan] VK_PRESENT_MODE_FIFO_KHR is not supported.");
  }

  scinfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  scinfo.surface = vulkan_->surface_;
  scinfo.minImageCount = 2;
  scinfo.imageFormat = surfaceFormats[formatIdx].format;
  scinfo.imageColorSpace = surfaceFormats[formatIdx].colorSpace;
  scinfo.imageExtent.width = width_;
  scinfo.imageExtent.height = height_;
  scinfo.imageArrayLayers = 1;
  scinfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  scinfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  scinfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  scinfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  scinfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
  scinfo.clipped = VK_TRUE;

  if (vkCreateSwapchainKHR(vulkan_->device_, &scinfo, nullptr, &vulkan_->swapchain_)) {
    log_.fatal("Failed to create swap chain.");
  }
}

void VulkanBuilder::createSwapchainImages() {
  vulkan_->swapchainImages_ = getSwapchainImages(vulkan_->device_, vulkan_->swapchain_);
  if (vulkan_->swapchainImages_.empty()) {
    log_.fatal("No swapchain images available.");
  }
}

void VulkanBuilder::createSwapchainImageViews() {
  vulkan_->swapchainImageViews_.resize(vulkan_->swapchainImages_.size());
  for (size_t i = 0; i < vulkan_->swapchainImages_.size(); ++i) {
    VkImageViewCreateInfo vinfo{};
    vinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    vinfo.image = vulkan_->swapchainImages_[i];
    vinfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    vinfo.format = VK_FORMAT_B8G8R8A8_UNORM;
    vinfo.components = {
        VK_COMPONENT_SWIZZLE_R,
        VK_COMPONENT_SWIZZLE_G,
        VK_COMPONENT_SWIZZLE_B,
        VK_COMPONENT_SWIZZLE_A,
    };
    vinfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    if (vkCreateImageView(vulkan_->device_, &vinfo, nullptr, &vulkan_->swapchainImageViews_[i]) != VK_SUCCESS) {
      log_.fatal("[Vulkan] Failed to create an image view");
    }
  }
}

void VulkanBuilder::createFrameBuffers() {
  for (size_t i = 0; i < this->vulkan_->swapchainImageViews_.size(); ++i) {
    std::shared_ptr<RenderPass> renderPass = util::make_shared<RenderPass>();
    {
      VkAttachmentDescription attachmentDesc{};
      VkAttachmentReference attachmentRef{};

      attachmentDesc.format = VK_FORMAT_B8G8R8A8_UNORM;
      attachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;
      attachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      attachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      attachmentDesc.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
      attachmentDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

      attachmentRef.attachment = 0;
      attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

      VkSubpassDescription subpass{};
      VkRenderPassCreateInfo renderPassInfo{};

      subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      subpass.colorAttachmentCount = 1;
      subpass.pColorAttachments = &attachmentRef;
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
      renderPassInfo.attachmentCount = 1;
      renderPassInfo.pAttachments = &attachmentDesc;
      renderPassInfo.subpassCount = 1;
      renderPassInfo.pSubpasses = &subpass;

      if (vkCreateRenderPass(vulkan_->device_, &renderPassInfo, nullptr, &renderPass->obj_) != VK_SUCCESS) {
        log_.fatal("[Vulkan] Failed to create a RenderPass.");
      }
    }
    std::shared_ptr<CommandBuffer> commandBuffer = util::make_shared<CommandBuffer>();
    {
      VkCommandBufferAllocateInfo cbAllocInfo{};

      cbAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      cbAllocInfo.commandPool = this->vulkan_->commandPool_;
      cbAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      cbAllocInfo.commandBufferCount = 1;

      if (vkAllocateCommandBuffers(vulkan_->device_, &cbAllocInfo, &commandBuffer->obj_) != VK_SUCCESS) {
        log_.fatal("[Vulkan] Failed to create a Command Buffer.");
      }
    }
    std::shared_ptr<FrameBuffer> frameBuffer = util::make_shared<FrameBuffer>(renderPass, std::move(commandBuffer));
    {
      VkFramebufferCreateInfo fbinfo{};
      VkImageView attachmentViews[1] = {vulkan_->swapchainImageViews_[i]};

      fbinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      fbinfo.attachmentCount = 1;
      fbinfo.renderPass = renderPass->obj_;
      fbinfo.pAttachments = attachmentViews;
      fbinfo.width = this->width_;
      fbinfo.height = this->height_;
      fbinfo.layers = 1;
      if (vkCreateFramebuffer(vulkan_->device_, &fbinfo, nullptr, &frameBuffer->obj_) != VK_SUCCESS) {
        log_.fatal("[Vulkan] Failed to create a FrameBuffer.");
      }
    }
    this->vulkan_->frameBuffers_.emplace_back(std::move(frameBuffer));
  }
}

void VulkanBuilder::createCommandBuffers() {

}

}
