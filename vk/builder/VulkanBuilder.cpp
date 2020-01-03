/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include <utility>
#include <vector>
#include <unordered_set>
#include "../Util.hpp"
#include "VulkanBuilder.hpp"
#include "../Vulkan.hpp"
#include "../RenderPass.hpp"
#include "../command/CommandPool.hpp"
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
  if((flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) == VK_DEBUG_REPORT_ERROR_BIT_EXT) {
    v->log().error("<< from Vulkan (layer={}, messageCode={}) >> \n{}", pLayerPrefix, messageCode, pMessage);
    // return VK_TRUE; // crash on error.
  }else{
    v->log().debug("<< from Vulkan (layer={}, messageCode={}) >> \n{}", pLayerPrefix, messageCode, pMessage);
  }
  return VK_FALSE;
}

}

VulkanBuilder::VulkanBuilder(util::Logger &log, std::string appName, int width, int height)
    : log_(log), appName_(std::move(appName)), width_(width), height_(height), vulkan_() {
}

std::shared_ptr<Vulkan> VulkanBuilder::build() {
  this->vulkan_ = std::make_shared<Vulkan>(this->log_);

  this->createWindow();
  this->createInstance();
  this->createSurface();
  this->createDebugReportCallback();
  this->createPhysicalDevice();

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

void VulkanBuilder::createPhysicalDevice() {
  std::vector<VkPhysicalDevice> physicalDevices = enumeratePhysicalDevices(vulkan()->vkInstance_);
  if (physicalDevices.empty()) {
    log().fatal("[Vulkan] No vulkan physical devices available.");
  }
  vulkan()->vkPhysicalDevice_ = nullptr;
  log().debug("[[Available Vulkan Physical Devices]]");
  for (VkPhysicalDevice device : physicalDevices) {
    VkPhysicalDeviceProperties props;
    VkPhysicalDeviceMemoryProperties memProps;
    vkGetPhysicalDeviceProperties(device, &props);
    vkGetPhysicalDeviceMemoryProperties(device, &memProps);
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);
    std::vector<VkQueueFamilyProperties> const properties = getPhysicalDeviceQueueFamilyProperties(device);

    std::optional<uint32_t> graphicsQueueFamilyIndex;
    std::optional<uint32_t> presentQueueFamilyIndex;
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

    /* *******************************************************************
     * Search queue family index for Graphics Queue and Present Queue
     * ******************************************************************* */

    for (size_t i = 0; i < properties.size(); ++i) {
      if ((properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
        if(!graphicsQueueFamilyIndex.has_value()){
          graphicsQueueFamilyIndex = i;
        }
      }
      VkBool32 surfaceSupported;
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vulkan()->vkSurface_, &surfaceSupported);
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
      log().warn("[Vulkan] No Graphics queues available.");
      continue;
    }

    if (!presentQueueFamilyIndex.has_value()) {
      log().fatal("[Vulkan] No Present queues available.");
      continue;
    }

    vulkan()->vkPhysicalDevice_ = device;
    vulkan()->vkGraphicsQueueFamilyIndex_ = graphicsQueueFamilyIndex.value();
    vulkan()->vkPresentQueueFamilyIndex_ = presentQueueFamilyIndex.value();
    vkGetPhysicalDeviceMemoryProperties(vulkan()->vkPhysicalDevice_, &vulkan()->vkPhysicalDeviceMemoryProperties_);
    break;
  }
  if(vulkan()->vkPhysicalDevice_ == nullptr) {
    log().fatal("No suitable physical device");
  }
}

}
