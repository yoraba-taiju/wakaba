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
#include "CommandPool.hpp"
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
  auto v = reinterpret_cast<Vulkan*>(pUserData);
  v->log().debug("[From Vulkan :: {}({})] \n{}", pLayerPrefix, messageCode, pMessage);
  return VK_FALSE;
}

}

VulkanBuilder::VulkanBuilder(util::Logger &log, std::string appName, int width, int height)
    : log_(log), appName_(std::move(appName)), width_(width), height_(height), vulkan_() {
}

std::shared_ptr<Vulkan> VulkanBuilder::create() {
  //FIXME: util::make_sharedつかったらshared_from_thisがうまく動かなかった
  this->vulkan_ = std::make_shared<Vulkan>(this->log_);

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

  vulkan_->width_ = width_;
  vulkan_->height_ = height_;
}

// Initialize Vulkan and create Instance.
void VulkanBuilder::createInstance() {
  VkInstanceCreateInfo instanceInfo{};
  VkApplicationInfo appInfo{};

  std::vector<std::string> const requiredExtensions = enumerateRequiredInstanceExtensions();
  uint32_t numExtensions = requiredExtensions.size() + 1;
  std::vector<const char*> extensionNames;
  log_.debug("[[Required Vulkan Extensions]]");
  for (size_t i = 0; i < requiredExtensions.size(); ++i) {
    extensionNames.emplace_back(requiredExtensions[i].c_str());
    log_.debug(" - {}", extensionNames[i]);
  }
  extensionNames.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

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
    log_.debug(" - {} ({} :: spec={}, impl={})",
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
  appInfo.pEngineName = "Wakaba Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_1;

  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.enabledExtensionCount = extensionNames.size();
  instanceInfo.ppEnabledExtensionNames = extensionNames.data();
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
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);
    log_.info(" - {}", props.deviceName);
    log_.info("   - properties: ");
    log_.info("     - API version: {:08x}", props.apiVersion);
    log_.info("     - driver version: {:08x}", props.driverVersion);
    log_.info("     - Vendor ID: {:08x}", props.vendorID);
    log_.info("     - Device ID: {:08x}", props.deviceID);
    log_.info("     - Device Type: {}", physicalDeviceTypeToString(props.deviceType));
    log_.info("   - features: ");
    log_.info("     - shaderFloat64: {}", features.shaderFloat64 ? "yes" : "no");
    log_.info("     - shaderInt16: {}", features.shaderInt16 ? "yes" : "no");
    log_.info("     - shaderInt64: {}", features.shaderInt64 ? "yes" : "no");
    log_.info("     - geometry shader: {}", features.geometryShader ? "yes" : "no");
    log_.info("     - tessellation shader: {}", features.tessellationShader ? "yes" : "no");
    log_.info("     - inherited queries: {}", features.inheritedQueries ? "yes" : "no");
  }
  vulkan_->physicalDevice_ = physicalDevices[0];
  {
    VkDeviceCreateInfo devInfo{};
    { // Search queue family index for Graphics Queue and Present Queue
      std::optional<uint32_t> graphicsQueueFamilyIndex;
      std::optional<uint32_t> presentQueueFamilyIndex;
      std::vector<VkQueueFamilyProperties> properties = getPhysicalDeviceQueueFamilyProperties(vulkan_->physicalDevice_);
      for (size_t i = 0; i < properties.size(); ++i) {
        if ((properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
          if(!graphicsQueueFamilyIndex.has_value()){
            graphicsQueueFamilyIndex = i;
          }
        }
        VkBool32 surfaceSupported;
        vkGetPhysicalDeviceSurfaceSupportKHR(vulkan_->physicalDevice_, i, vulkan_->surface_, &surfaceSupported);
        if(VK_TRUE == surfaceSupported) {
          if(!presentQueueFamilyIndex.has_value()) {
            presentQueueFamilyIndex = i;
          }
        }
        if(graphicsQueueFamilyIndex.has_value() && presentQueueFamilyIndex.has_value()) {
          break;
        }
      }
      if (!graphicsQueueFamilyIndex.has_value()) {
        log_.fatal("[Vulkan] No Graphics queues available.");
      }
      if (!presentQueueFamilyIndex) {
        log_.fatal("[Vulkan] Physical device does not support swapchain.");
      }
      vulkan_->graphicsQueueFamiliIndex_ = graphicsQueueFamilyIndex.value();
      vulkan_->presentQueueFamiliIndex_ = presentQueueFamilyIndex.value();
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    {
      static float qPriorities[] = {1.0f};
      // Graphics queue
      queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
          .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
          .pNext = nullptr,
          .queueFamilyIndex = vulkan_->graphicsQueueFamiliIndex_,
          .queueCount = 1,
          .pQueuePriorities = qPriorities,
      });
      if(vulkan_->graphicsQueueFamiliIndex_ != vulkan_->presentQueueFamiliIndex_) {
        // Present queue
        queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .queueFamilyIndex = vulkan_->presentQueueFamiliIndex_,
            .queueCount = 1,
            .pQueuePriorities = qPriorities,
        });
      }
    }
    devInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    devInfo.queueCreateInfoCount = queueCreateInfos.size();
    devInfo.pQueueCreateInfos = queueCreateInfos.data();
    // enabledLayerCount is deprecated and ignored.
    // ppEnabledLayerNames is deprecated and ignored.
    // See https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#extendingvulkan-layers-devicelayerdeprecation.
    devInfo.enabledLayerCount = 0;
    devInfo.ppEnabledLayerNames = nullptr;
    const char *extensions[] = {"VK_KHR_swapchain"};
    devInfo.enabledExtensionCount = std::size(extensions);
    devInfo.ppEnabledExtensionNames = extensions;

    if (vkCreateDevice(vulkan_->physicalDevice_, &devInfo, nullptr, &vulkan_->device_) != VK_SUCCESS) {
      log_.fatal("[Vulkan] Failed to create a device.");
    }
    vkGetDeviceQueue(vulkan_->device_, vulkan_->graphicsQueueFamiliIndex_, 0, &vulkan_->graphicsQueue_);
    vkGetDeviceQueue(vulkan_->device_, vulkan_->presentQueueFamiliIndex_, 0, &vulkan_->presentQueue_);
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
  VkSwapchainCreateInfoKHR swapchainCreateInfo{};
  VkSurfaceCapabilitiesKHR surfaceCaps{};
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan_->physicalDevice_, vulkan_->surface_, &surfaceCaps);

  auto surfaceFormats = getPhysicalDeviceSurfaceFormats(vulkan_->physicalDevice_, vulkan_->surface_);
  auto presentModes = getPhysicalDeviceSurfacePresentModes(vulkan_->physicalDevice_, vulkan_->surface_);

  std::optional<size_t> formatIdx;
  for (size_t i = 0; i < surfaceFormats.size(); ++i) {
    auto format = surfaceFormats[i];
    if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
      formatIdx = i;
      break;
    }
  }
  if (!formatIdx.has_value()) {
    log_.fatal("[Vulkan] VK_FORMAT_B8G8R8A8_UNORM & VK_COLORSPACE_SRGB_NONLINEAR_KHR is not supported.");
  }
  vulkan_->swapchainFormat_ = surfaceFormats[formatIdx.value()];

  std::optional<size_t> presentModeIdx;
  for (size_t i = 0; i < presentModes.size(); ++i) {
    auto mode = presentModes[i];
    if (mode == VK_PRESENT_MODE_FIFO_KHR) {
      presentModeIdx = i;
    }
  }

  if (!presentModeIdx.has_value()) {
    log_.fatal("[Vulkan] VK_PRESENT_MODE_FIFO_KHR is not supported.");
  }

  swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainCreateInfo.surface = vulkan_->surface_;
  swapchainCreateInfo.minImageCount = surfaceCaps.minImageCount + 1;
  swapchainCreateInfo.imageFormat = surfaceFormats[formatIdx.value()].format; // VK_FORMAT_B8G8R8A8_UNORM
  swapchainCreateInfo.imageColorSpace = surfaceFormats[formatIdx.value()].colorSpace; // VK_COLORSPACE_SRGB_NONLINEAR_KHR
  swapchainCreateInfo.imageExtent.width = vulkan_->width();
  swapchainCreateInfo.imageExtent.height = vulkan_->height();
  swapchainCreateInfo.imageArrayLayers = 1;
  swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  std::vector<uint32_t> queueFamilyIndex;
  if (vulkan_->graphicsQueueFamiliIndex_ != vulkan_->presentQueueFamiliIndex_) {
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    queueFamilyIndex.emplace_back(vulkan_->graphicsQueueFamiliIndex_);
    queueFamilyIndex.emplace_back(vulkan_->presentQueueFamiliIndex_);
    swapchainCreateInfo.queueFamilyIndexCount = queueFamilyIndex.size();
    swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndex.data();
  } else {
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.queueFamilyIndexCount = 0; // Optional
    swapchainCreateInfo.pQueueFamilyIndices = nullptr; // Optional
  }
  swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchainCreateInfo.preTransform = surfaceCaps.currentTransform;
  swapchainCreateInfo.presentMode = presentModes[presentModeIdx.value()]; //VK_PRESENT_MODE_FIFO_KHR
  swapchainCreateInfo.clipped = VK_TRUE;
  swapchainCreateInfo.oldSwapchain = nullptr;

  if (vkCreateSwapchainKHR(vulkan_->device_, &swapchainCreateInfo, nullptr, &vulkan_->swapchain_)) {
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
    VkImageViewCreateInfo imageViewCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = vulkan_->swapchainImages_[i],
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_B8G8R8A8_UNORM,
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
    if (vkCreateImageView(vulkan_->device_, &imageViewCreateInfo, nullptr, &vulkan_->swapchainImageViews_[i]) != VK_SUCCESS) {
      log_.fatal("[Vulkan] Failed to create an image view");
    }
  }
}

void VulkanBuilder::createFrameBuffers() {
  for (size_t i = 0; i < this->vulkan_->swapchainImageViews_.size(); ++i) {
    VkRenderPass pass;
    {
      VkAttachmentDescription attachmentDesc{};
      VkAttachmentReference attachmentRef{};

      attachmentDesc.format = vulkan_->swapchainFormat_.format;
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

      if (vkCreateRenderPass(vulkan_->device_, &renderPassInfo, nullptr, &pass) != VK_SUCCESS) {
        log_.fatal("[Vulkan] Failed to create a RenderPass.");
      }
    }
    std::shared_ptr<RenderPass> renderPass = util::make_shared<RenderPass>(vulkan_, pass);

    VkCommandPool vkCommandPool;
    {
      VkCommandPoolCreateInfo info{};

      info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      info.queueFamilyIndex = vulkan_->graphicsQueueFamiliIndex_;
      info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

      if (vkCreateCommandPool(vulkan_->device_, &info, nullptr, &vkCommandPool) != VK_SUCCESS) {
        log_.fatal("[Vulkan] Failed to create a command pool.");
      }
    }
    std::shared_ptr<CommandPool> commandPool = util::make_shared<CommandPool>(vulkan_, vkCommandPool);

    VkCommandBuffer vkCommandBuffer;
    {
      VkCommandBufferAllocateInfo cbAllocInfo{};

      cbAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      cbAllocInfo.commandPool = commandPool->vkObj();
      cbAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      cbAllocInfo.commandBufferCount = 1;

      if (vkAllocateCommandBuffers(vulkan_->device_, &cbAllocInfo, &vkCommandBuffer) != VK_SUCCESS) {
        log_.fatal("[Vulkan] Failed to create a Command Buffer.");
      }
    }
    std::shared_ptr<CommandBuffer> commandBuffer = util::make_shared<CommandBuffer>(vulkan_, commandPool, vkCommandBuffer);

    VkFramebuffer vkFramebuffer;
    {
      VkFramebufferCreateInfo fbinfo{};
      VkImageView attachmentViews[1] = {vulkan_->swapchainImageViews_[i]};

      fbinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      fbinfo.attachmentCount = 1;
      fbinfo.renderPass = renderPass->vkObj();
      fbinfo.pAttachments = attachmentViews;
      fbinfo.width = vulkan_->width();
      fbinfo.height = vulkan_->height();
      fbinfo.layers = 1;
      if (vkCreateFramebuffer(vulkan_->device_, &fbinfo, nullptr, &vkFramebuffer) != VK_SUCCESS) {
        log_.fatal("[Vulkan] Failed to create a FrameBuffer.");
      }
    }
    this->vulkan_->frameBuffers_.emplace_back(
      util::make_shared<FrameBuffer>(vulkan_, vkFramebuffer, std::move(renderPass), std::move(commandPool), std::move(commandBuffer))
    );
  }
}

}
