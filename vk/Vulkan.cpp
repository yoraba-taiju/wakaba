//
// Created by kaede on 2019/10/29.
//

#include <memory>
#include <vector>

#include "Vulkan.hpp"
#include "Util.hpp"

namespace vk {

//Builderみたいなのほしい

std::shared_ptr<Vulkan> Vulkan::createInstance(util::Logger& log, std::string const& appName) {
  auto v = util::make_shared<Vulkan>();
  {
    // Open window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    v->window_ = glfwCreateWindow(1920, 1080, "YorabaTaiju", nullptr, nullptr);

    if (v->window_ == nullptr) {
      glfwTerminate();
      log.fatal("Failed to open GLFW window.");
    }
  }

  {
    // Initialize Vulkan;
    VkInstanceCreateInfo instanceInfo{};
    VkApplicationInfo appInfo{};

    std::vector<std::string> const extensions =  enumurateRequiredInstanceExtensions();
    const char* extensionNames[extensions.size()];
    for(size_t i = 0; i < extensions.size(); ++i) {
      extensionNames[i] = extensions[i].c_str();
    }

    std::vector<VkLayerProperties> layers = enumerateInstanceLayerProperties();
    const char* layerNames[layers.size()];
    for(size_t i = 0; i < layers.size(); ++i) {
      layerNames[i] = layers[i].layerName;
    }
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 1, 126);
    appInfo.pApplicationName = appName.c_str();
    appInfo.apiVersion = VK_API_VERSION_1_1;

    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = extensions.size();
    instanceInfo.ppEnabledExtensionNames = extensionNames;
    instanceInfo.enabledLayerCount = layers.size();
    instanceInfo.ppEnabledLayerNames = layerNames;

    VkResult const res = vkCreateInstance(&instanceInfo, nullptr, &v->instance_);
    if(res != VK_SUCCESS) {
      log.fatal("Failed to create Vulkan Instance");
    }
  }

  {
    // createWindowSurface
    VkResult const res = glfwCreateWindowSurface 	(v->instance_, v->window_, nullptr, &v->surface_);
    if(res != VK_SUCCESS) {
      log.fatal("Failed to create Vulkan Surface");
    }
  }

  return std::move(v);
}

Vulkan::Vulkan():
instance_()
{

}

}