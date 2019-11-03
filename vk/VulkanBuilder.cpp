//
// Created by kaede on 2019/10/30.
//

#include <utility>
#include <vector>
#include "../util/Shared.hpp"
#include "Util.hpp"
#include "VulkanBuilder.hpp"
#include "Vulkan.hpp"

namespace vk {

namespace {

VKAPI_ATTR VkBool32 VKAPI_CALL
onError(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT, uint64_t object, size_t location, int32_t messageCode,
        const char *pLayerPrefix, const char *pMessage, void *pUserData) {
  auto vk = reinterpret_cast<Vulkan*>(pUserData);
  vk->log().debug("[Vk] %s", pMessage);
  return VK_FALSE;
}

}

VulkanBuilder::VulkanBuilder(util::Logger& log, std::string appName)
:log_(log)
,appName_(std::move(appName))
,vulkan_()
{
}

std::shared_ptr<Vulkan> VulkanBuilder::create() {
  this->vulkan_ = std::move(util::make_shared<Vulkan>(this->log_));

  this->createWindow();
  this->createInstance();
  this->createSurface();
  this->createDebugReportCallback();
  this->createDeviceAndCommandPool();
  this->createFence();

  return this->vulkan_;
}

// Open window
void VulkanBuilder::createWindow() {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  this->vulkan_->window_ = glfwCreateWindow(1920, 1080, "YorabaTaiju", nullptr, nullptr);

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
  const char* extensionNames[numExtensions];
  log_.debug("[[Required Vulkan Extensions]]");
  for(size_t i = 0; i < requiredExtensions.size(); ++i) {
    extensionNames[i] = requiredExtensions[i].c_str();
    log_.debug(" - %s", extensionNames[i]);
  }
  extensionNames[numExtensions - 1] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;

  std::vector<VkLayerProperties> layers = vk::enumerateInstanceLayerProperties();
  const char* layerNames[layers.size()];
  log_.debug("[[Available Vulkan Layers]]");
  for(size_t i = 0; i < layers.size(); ++i) {
    layerNames[i] = layers[i].layerName;
    log_.debug(" - %s (%s :: spec=%d, impl=%d)",
               layers[i].layerName,
               layers[i].description,
               layers[i].specVersion,
               layers[i].implementationVersion);
  }

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 1, 126);
  appInfo.pApplicationName = this->appName_.c_str();
  appInfo.apiVersion = VK_API_VERSION_1_1;

  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.enabledExtensionCount = requiredExtensions.size() + 1;
  instanceInfo.ppEnabledExtensionNames = extensionNames;
  instanceInfo.enabledLayerCount = layers.size();
  instanceInfo.ppEnabledLayerNames = layerNames;

  VkResult const res = vkCreateInstance(&instanceInfo, nullptr, &this->vulkan_->instance_);
  if(res != VK_SUCCESS) {
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
  if(res != VK_SUCCESS) {
    log_.fatal("Failed to create Vulkan Surface");
  }
}

// Create vulkan's debug reporter callback.
void VulkanBuilder::createDebugReportCallback() {
  VkDebugReportCallbackCreateInfoEXT callbackInfo{};
  callbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  callbackInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;// | VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
  callbackInfo.pfnCallback = &onError;
  callbackInfo.pUserData = vulkan_.get();

  auto _vkCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)(glfwGetInstanceProcAddress(this->vulkan_->instance_, "vkCreateDebugReportCallbackEXT"));
  if (_vkCreateDebugReportCallback == nullptr) {
    log_.fatal("[Vulkan] vkCreateDebugReportCallbackEXT not available.");
  }
  auto _vkDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)(glfwGetInstanceProcAddress(this->vulkan_->instance_, "vkDestroyDebugReportCallbackEXT"));
  if (_vkDestroyDebugReportCallback == nullptr) {
    log_.fatal("[Vulkan] vkDestroyDebugReportCallbackEXT not available.");
  }
  auto res = _vkCreateDebugReportCallback(vulkan_->instance_, &callbackInfo, nullptr, &vulkan_->vkDebugReportCallback_);
  if(res != VK_SUCCESS) {
    log_.fatal("Failed initialize a debug reporter.");
  }
  vulkan_->vkDestroyDebugReportCallback_ = _vkDestroyDebugReportCallback;
}

void VulkanBuilder::createDeviceAndCommandPool() {
  std::vector<VkPhysicalDevice> physicalDevices = enumeratePhysicalDevices(vulkan_->instance_);
  if(physicalDevices.empty()) {
    log_.fatal("[Vulkan] No vulkan physical devices available.");
  }
  log_.debug("[[Available Vulkan Physical Devices]]");
  for(VkPhysicalDevice device : physicalDevices) {
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
    VkPhysicalDevice physicalDevice = physicalDevices[0];

    // Search queue family index for Graphics Queue
    uint32_t queueFamilyIndex = 0xffffffff;
    std::vector<VkQueueFamilyProperties> properties = getPhysicalDeviceQueueFamilyProperties(physicalDevice);
    for (size_t i = 0; i < properties.size(); i++)
    {
      if ((properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
        queueFamilyIndex = i;
        break;
      }
    }
    if (queueFamilyIndex == 0xffffffff) {
      log_.fatal("No Graphics queues available.");
    }

    const char* layers[] = { "VK_LAYER_LUNARG_standard_validation" };
    const char* extensions[] = { "VK_KHR_swapchain" };
    static float qPriorities[] = { 0.0f };
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueCount = 1;
    queueInfo.queueFamilyIndex = queueFamilyIndex;
    queueInfo.pQueuePriorities = qPriorities;
    devInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    devInfo.queueCreateInfoCount = 1;
    devInfo.pQueueCreateInfos = &queueInfo;
    devInfo.enabledLayerCount = std::size(layers);
    devInfo.ppEnabledLayerNames = layers;
    devInfo.enabledExtensionCount = std::size(extensions);
    devInfo.ppEnabledExtensionNames = extensions;

    auto res = vkCreateDevice(physicalDevice, &devInfo, nullptr, &vulkan_->device_);
    if(res != VK_SUCCESS) {
      log_.fatal("[Vulkan] Failed to create a device.");
    }

    VkCommandPoolCreateInfo info{};

    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.queueFamilyIndex = queueFamilyIndex;
    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if(vkCreateCommandPool(vulkan_->device_, &info, nullptr, &vulkan_->commandPool_) != VK_SUCCESS) {
      log_.fatal("[Vulkan] Failed to create a command pool.");
    }
  }
}

void VulkanBuilder::createFence() {
  VkFenceCreateInfo finfo{};
  finfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  if(vkCreateFence(vulkan_->device_, &finfo, nullptr, &vulkan_->fence_) != VK_SUCCESS) {
    log_.fatal("[Vulkan] Failed to create a fence.");
  }
}


}
