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

VulkanBuilder::VulkanBuilder(util::Logger& log, std::string appName)
:log_(log)
,appName_(std::move(appName))
,vulkan_()
{
  this->vulkan_ = std::move(util::make_shared<Vulkan>());
  }

std::shared_ptr<Vulkan> VulkanBuilder::create() {
  {
    // Open window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    this->vulkan_->window_ = glfwCreateWindow(1920, 1080, "YorabaTaiju", nullptr, nullptr);

    if (this->vulkan_->window_ == nullptr) {
      glfwTerminate();
      log_.fatal("Failed to open GLFW window.");
    }
  }

  {
    // Initialize Vulkan;
    VkInstanceCreateInfo instanceInfo{};
    VkApplicationInfo appInfo{};

    std::vector<std::string> const extensions = enumurateRequiredInstanceExtensions();
    const char* extensionNames[extensions.size()];
    for(size_t i = 0; i < extensions.size(); ++i) {
      extensionNames[i] = extensions[i].c_str();
      log_.debug("Vulkan Required Extention: %s", extensionNames[i]);
    }

    std::vector<VkLayerProperties> layers = vk::enumerateInstanceLayerProperties();
    const char* layerNames[layers.size()];
    for(size_t i = 0; i < layers.size(); ++i) {
      layerNames[i] = layers[i].layerName;
      log_.debug("Layer: %s (%s :: spec=%d, impl=%d)",
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
    instanceInfo.enabledExtensionCount = extensions.size();
    instanceInfo.ppEnabledExtensionNames = extensionNames;
    instanceInfo.enabledLayerCount = layers.size();
    instanceInfo.ppEnabledLayerNames = layerNames;

    VkResult const res = vkCreateInstance(&instanceInfo, nullptr, &this->vulkan_->instance_);
    if(res != VK_SUCCESS) {
      log_.fatal("Failed to create Vulkan Instance");
    }
  }

  {
    // createWindowSurface
    VkResult const res = glfwCreateWindowSurface(
        this->vulkan_->instance_,
        this->vulkan_->window_,
        nullptr,
        &this->vulkan_->surface_);
    if(res != VK_SUCCESS) {
      log_.fatal("Failed to create Vulkan Surface");
    }
  }

  return this->vulkan_;
}


}
