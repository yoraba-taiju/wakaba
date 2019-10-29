//
// Created by kaede on 2019/10/29.
//

#include <memory>

#include "Vulkan.hpp"

namespace vk {

std::shared_ptr<Vulkan> Vulkan::createInstance(std::string const& appName) {
  VkInstanceCreateInfo instanceInfo{};
  VkApplicationInfo appInfo{};
  static const char* extensions[] =
      {
          VK_KHR_SURFACE_EXTENSION_NAME,
          VK_EXT_DEBUG_REPORT_EXTENSION_NAME
      };
  static const char* layers[] = { "VK_LAYER_LUNARG_standard_validation" };

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
  appInfo.pApplicationName = appName.c_str();
  appInfo.apiVersion = VK_API_VERSION_1_1;

  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.enabledExtensionCount = std::size(extensions);
  instanceInfo.ppEnabledExtensionNames = extensions;
  instanceInfo.enabledLayerCount = std::size(layers);
  instanceInfo.ppEnabledLayerNames = layers;

  VkInstance instance;
  auto v = util::make_shared<Vulkan>();
  VkResult const res = vkCreateInstance(&instanceInfo, nullptr, &v->instance_);
  return std::move(v);
}

Vulkan::Vulkan():
instance_()
{

}

}