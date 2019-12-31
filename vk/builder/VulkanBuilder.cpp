/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include <utility>
#include <vector>
#include <unordered_set>
#include "../../util/Shared.hpp"
#include "../Util.hpp"
#include "VulkanBuilder.hpp"
#include "../Vulkan.hpp"
#include "../RenderPass.hpp"
#include "../FrameBuffer.hpp"
#include "../CommandPool.hpp"
#include "../CommandBuffer.hpp"
#include "../image/Image.hpp"
#include "../image/SwapchainImage.hpp"

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

std::shared_ptr<Vulkan> VulkanBuilder::build() {
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
  this->createFrameBuffers();

  return this->vulkan_;
}

// Open window
void VulkanBuilder::createWindow() {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  this->vulkan()->window_ = glfwCreateWindow(width_, height_, "YorabaTaiju", nullptr, nullptr);

  if (this->vulkan()->window_ == nullptr) {
    glfwTerminate();
    log().fatal("Failed to open GLFW window.");
  }

  vulkan()->width_ = width_;
  vulkan()->height_ = height_;
}

// Initialize Vulkan and create Instance.
void VulkanBuilder::createInstance() {
  VkInstanceCreateInfo instanceInfo{};
  VkApplicationInfo appInfo{};

  std::vector<std::string> const requiredExtensions = enumerateRequiredInstanceExtensions();
  uint32_t numExtensions = requiredExtensions.size() + 1;
  std::vector<const char*> extensionNames;
  log().debug("[[Required Vulkan Extensions]]");
  for (size_t i = 0; i < requiredExtensions.size(); ++i) {
    extensionNames.emplace_back(requiredExtensions[i].c_str());
    log().debug(" - {}", extensionNames[i]);
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
  log().debug("[[Available Vulkan Layers]]");
  for (auto & layer : layers) {
    log().debug(" - {} ({} :: spec={}, impl={})",
               layer.layerName,
               layer.description,
               layer.specVersion,
               layer.implementationVersion);
    if(desiredLayers.find(layer.layerName) != desiredLayers.end()) {
      enabledLayers.emplace_back(layer.layerName);
      log().debug("   - enabled!");
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

  if (vkCreateInstance(&instanceInfo, nullptr, &this->vulkan()->vkInstance_) != VK_SUCCESS) {
    log().fatal("Failed to create Vulkan Instance");
  }
}

// Attach the instance to the window.
void VulkanBuilder::createSurface() {
  VkResult const res = glfwCreateWindowSurface(
      this->vulkan()->vkInstance_,
      this->vulkan()->window_,
      nullptr,
      &this->vulkan()->vkSurface_);
  if (res != VK_SUCCESS) {
    log().fatal("Failed to create Vulkan Surface");
  }
}

// Create vulkan's debug reporter callback.
void VulkanBuilder::createDebugReportCallback() {
  VkDebugReportCallbackCreateInfoEXT callbackInfo{
      .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
      .pNext = nullptr,
      .flags =
          VK_DEBUG_REPORT_ERROR_BIT_EXT |
          VK_DEBUG_REPORT_WARNING_BIT_EXT |
          VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT, // | VK_DEBUG_REPORT_INFORMATION_BIT_EXT
      .pfnCallback = &onError,
      .pUserData = vulkan_.get(),
  };

  auto _vkCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT) (glfwGetInstanceProcAddress(
      this->vulkan()->vkInstance_, "vkCreateDebugReportCallbackEXT"));
  if (_vkCreateDebugReportCallback == nullptr) {
    log().fatal("[Vulkan] vkCreateDebugReportCallbackEXT not available.");
  }
  auto _vkDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT) (glfwGetInstanceProcAddress(
      this->vulkan()->vkInstance_, "vkDestroyDebugReportCallbackEXT"));
  if (_vkDestroyDebugReportCallback == nullptr) {
    log().fatal("[Vulkan] vkDestroyDebugReportCallbackEXT not available.");
  }
  if (_vkCreateDebugReportCallback(vulkan()->vkInstance_, &callbackInfo, nullptr, &vulkan()->vkDebugReportCallback_) !=
      VK_SUCCESS) {
    log().fatal("Failed initialize a debug reporter.");
  }
  vulkan()->vkDestroyDebugReportCallback_ = _vkDestroyDebugReportCallback;
}

void VulkanBuilder::createDeviceAndCommandPool() {
  std::vector<VkPhysicalDevice> physicalDevices = enumeratePhysicalDevices(vulkan()->vkInstance_);
  if (physicalDevices.empty()) {
    log().fatal("[Vulkan] No vulkan physical devices available.");
  }
  log().debug("[[Available Vulkan Physical Devices]]");
  for (VkPhysicalDevice device : physicalDevices) {
    VkPhysicalDeviceProperties props;
    VkPhysicalDeviceMemoryProperties memProps;
    vkGetPhysicalDeviceProperties(device, &props);
    vkGetPhysicalDeviceMemoryProperties(device, &memProps);
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);
    log().info(" - {}", props.deviceName);
    log().info("   - properties: ");
    log().info("     - API version: {:08x}", props.apiVersion);
    log().info("     - driver version: {:08x}", props.driverVersion);
    log().info("     - Vendor ID: {:08x}", props.vendorID);
    log().info("     - Device ID: {:08x}", props.deviceID);
    log().info("     - Device Type: {}", physicalDeviceTypeToString(props.deviceType));
    log().info("   - features: ");
    log().info("     - shaderFloat64: {}", features.shaderFloat64 ? "yes" : "no");
    log().info("     - shaderInt16: {}", features.shaderInt16 ? "yes" : "no");
    log().info("     - shaderInt64: {}", features.shaderInt64 ? "yes" : "no");
    log().info("     - geometry shader: {}", features.geometryShader ? "yes" : "no");
    log().info("     - tessellation shader: {}", features.tessellationShader ? "yes" : "no");
    log().info("     - inherited queries: {}", features.inheritedQueries ? "yes" : "no");
  }
  vulkan()->vkPhysicalDevice_ = physicalDevices[0];
  {
    { // Search queue family index for Graphics Queue and Present Queue
      std::optional<uint32_t> graphicsQueueFamilyIndex;
      std::optional<uint32_t> presentQueueFamilyIndex;
      std::vector<VkQueueFamilyProperties> properties = getPhysicalDeviceQueueFamilyProperties(vulkan()->vkPhysicalDevice_);
      for (size_t i = 0; i < properties.size(); ++i) {
        if ((properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
          if(!graphicsQueueFamilyIndex.has_value()){
            graphicsQueueFamilyIndex = i;
          }
        }
        VkBool32 surfaceSupported;
        vkGetPhysicalDeviceSurfaceSupportKHR(vulkan()->vkPhysicalDevice_, i, vulkan()->vkSurface_, &surfaceSupported);
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
        log().fatal("[Vulkan] No Graphics queues available.");
      }
      if (!presentQueueFamilyIndex) {
        log().fatal("[Vulkan] Physical device does not support swapchain.");
      }
      vulkan()->graphicsQueueFamiliIndex_ = graphicsQueueFamilyIndex.value();
      vulkan()->presentQueueFamiliIndex_ = presentQueueFamilyIndex.value();
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    {
      static float qPriorities[] = {1.0f};
      // Graphics queue
      queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
          .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
          .pNext = nullptr,
          .flags = 0,
          .queueFamilyIndex = vulkan()->graphicsQueueFamiliIndex_,
          .queueCount = 1,
          .pQueuePriorities = qPriorities,
      });
      if(vulkan()->graphicsQueueFamiliIndex_ != vulkan()->presentQueueFamiliIndex_) {
        // Present queue
        queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = vulkan()->presentQueueFamiliIndex_,
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

    if (vkCreateDevice(vulkan()->vkPhysicalDevice_, &devInfo, nullptr, &vulkan()->vkDevice_) != VK_SUCCESS) {
      log().fatal("[Vulkan] Failed to create a device.");
    }
    vkGetDeviceQueue(vulkan()->vkDevice_, vulkan()->graphicsQueueFamiliIndex_, 0, &vulkan()->vkGraphicsQueue_);
    vkGetDeviceQueue(vulkan()->vkDevice_, vulkan()->presentQueueFamiliIndex_, 0, &vulkan()->vkPresentQueue_);
  }
}

void VulkanBuilder::createFence() {
  VkFenceCreateInfo finfo{};
  finfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  if (vkCreateFence(vulkan()->vkDevice_, &finfo, nullptr, &vulkan()->fence_) != VK_SUCCESS) {
    log().fatal("[Vulkan] Failed to create a fence.");
  }
}

void VulkanBuilder::createSwapchain() {
  VkSurfaceCapabilitiesKHR surfaceCaps{};
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan()->vkPhysicalDevice_, vulkan()->vkSurface_, &surfaceCaps);

  auto surfaceFormats = getPhysicalDeviceSurfaceFormats(vulkan()->vkPhysicalDevice_, vulkan()->vkSurface_);
  auto presentModes = getPhysicalDeviceSurfacePresentModes(vulkan()->vkPhysicalDevice_, vulkan()->vkSurface_);

  std::optional<size_t> formatIdx;
  for (size_t i = 0; i < surfaceFormats.size(); ++i) {
    auto format = surfaceFormats[i];
    if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
      formatIdx = i;
      break;
    }
  }
  if (!formatIdx.has_value()) {
    log().fatal("[Vulkan] VK_FORMAT_B8G8R8A8_UNORM & VK_COLORSPACE_SRGB_NONLINEAR_KHR is not supported.");
  }
  vulkan()->vkSwapchainFormat_ = surfaceFormats[formatIdx.value()];

  std::optional<size_t> presentModeIdx;
  for (size_t i = 0; i < presentModes.size(); ++i) {
    auto mode = presentModes[i];
    if (mode == VK_PRESENT_MODE_FIFO_KHR) {
      presentModeIdx = i;
    }
  }

  if (!presentModeIdx.has_value()) {
    log().fatal("[Vulkan] VK_PRESENT_MODE_FIFO_KHR is not supported.");
  }

  VkSwapchainCreateInfoKHR swapchainCreateInfo{};
  swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainCreateInfo.surface = vulkan()->vkSurface_;
  swapchainCreateInfo.minImageCount = surfaceCaps.minImageCount + 1;
  swapchainCreateInfo.imageFormat = surfaceFormats[formatIdx.value()].format; // VK_FORMAT_B8G8R8A8_UNORM
  swapchainCreateInfo.imageColorSpace = surfaceFormats[formatIdx.value()].colorSpace; // VK_COLORSPACE_SRGB_NONLINEAR_KHR
  swapchainCreateInfo.imageExtent.width = vulkan()->width();
  swapchainCreateInfo.imageExtent.height = vulkan()->height();
  swapchainCreateInfo.imageArrayLayers = 1;
  swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  std::vector<uint32_t> queueFamilyIndex;
  if (vulkan()->graphicsQueueFamiliIndex_ != vulkan()->presentQueueFamiliIndex_) {
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    queueFamilyIndex.emplace_back(vulkan()->graphicsQueueFamiliIndex_);
    queueFamilyIndex.emplace_back(vulkan()->presentQueueFamiliIndex_);
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

  if (vkCreateSwapchainKHR(vulkan()->vkDevice_, &swapchainCreateInfo, nullptr, &vulkan()->vkSwapchain_)) {
    log().fatal("Failed to create swap chain.");
  }
}

void VulkanBuilder::createSwapchainImages() {
  std::vector<VkImage> vkImages = getSwapchainImages(vulkan()->vkDevice_, vulkan()->vkSwapchain_);
  if (vkImages.empty()) {
    log().fatal("No swapchain images available.");
  }
  for(VkImage& vkImage : vkImages) {
    VkImageViewCreateInfo imageViewCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .image = vkImage,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = vulkan()->vkSwapchainFormat_.format,
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
    if (vkCreateImageView(vulkan()->vkDevice_, &imageViewCreateInfo, nullptr, &vkImageView) != VK_SUCCESS) {
      log().fatal("[Vulkan] Failed to create an image view");
    }
    vulkan()->swapchainImages_.emplace_back(std::make_shared<SwapchainImage>(vulkan(), vkImage, vkImageView));
  }
}

void VulkanBuilder::createFrameBuffers() {
  /*
    VkFramebuffer vkFramebuffer;
    {
      VkFramebufferCreateInfo fbinfo{};
      VkImageView attachmentViews[1] = {vulkan()->swapchainImageViews_[i]};

      fbinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      fbinfo.attachmentCount = 1;
      fbinfo.renderPass = renderPass->vkRenderPass();
      fbinfo.pAttachments = attachmentViews;
      fbinfo.width = vulkan()->width();
      fbinfo.height = vulkan()->height();
      fbinfo.layers = 1;
      if (vkCreateFramebuffer(vulkan()->vkDevice(), &fbinfo, nullptr, &vkFramebuffer) != VK_SUCCESS) {
        log().fatal("[Vulkan] Failed to create a FrameBuffer.");
      }
    }
    this->vulkan()->frameBuffers_.emplace_back(
      util::make_shared<FrameBuffer>(vulkan(), vkFramebuffer, std::move(renderPass), std::move(commandPool), std::move(commandBuffer))
    );
  }
   */
}

}
