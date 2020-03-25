/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Util.hpp"

namespace vk {

std::vector<std::string> enumerateRequiredInstanceExtensions() {
  uint32_t numExtensions;
  const char **const extensions = glfwGetRequiredInstanceExtensions(&numExtensions);
  std::vector<std::string> res;
  res.reserve(numExtensions);
  for (uint32_t i = 0; i < numExtensions; ++i) {
    res.emplace_back(extensions[i]);
  }
  return res;
}

std::vector<VkLayerProperties> enumerateInstanceLayerProperties() {
  uint32_t numProps = 0;
  {
    VkResult const result = vkEnumerateInstanceLayerProperties(&numProps, nullptr);
    if (result != VK_SUCCESS) {
      return std::vector<VkLayerProperties>();
    }
  }
  std::vector<VkLayerProperties> props;
  props.resize(numProps);
  VkResult const result = vkEnumerateInstanceLayerProperties(&numProps, props.data());
  if (result != VK_SUCCESS) {
    return std::vector<VkLayerProperties>();
  }
  return props;
}

std::vector<VkPhysicalDevice> enumeratePhysicalDevices(VkInstance instance) {
  uint32_t numDevices;
  VkResult result = vkEnumeratePhysicalDevices(instance, &numDevices, nullptr);
  if (result != VK_SUCCESS) {
    return std::vector<VkPhysicalDevice>();
  }
  std::vector<VkPhysicalDevice> devices;
  devices.resize(numDevices);
  result = vkEnumeratePhysicalDevices(instance, &numDevices, devices.data());
  if (result != VK_SUCCESS) {
    return std::vector<VkPhysicalDevice>();
  }
  return devices;
}

std::vector<VkQueueFamilyProperties> getPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice) {
  uint32_t numProps;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &numProps, nullptr);
  std::vector<VkQueueFamilyProperties> props;
  props.resize(numProps);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &numProps, props.data());
  return props;
}

std::vector<VkSurfaceFormatKHR> getPhysicalDeviceSurfaceFormats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
  std::vector<VkSurfaceFormatKHR> surfaceFormats;
  uint32_t numSurfaceFormats;
  vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &numSurfaceFormats, nullptr);
  surfaceFormats.resize(numSurfaceFormats);
  vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &numSurfaceFormats, surfaceFormats.data());
  return surfaceFormats;
}

std::vector<VkPresentModeKHR>
getPhysicalDeviceSurfacePresentModes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
  std::vector<VkPresentModeKHR> presentModes;
  uint32_t numPresentModes;
  vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &numPresentModes, nullptr);
  presentModes.resize(numPresentModes);
  vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &numPresentModes, presentModes.data());
  return presentModes;
}

std::vector<VkImage> getSwapchainImages(VkDevice device, VkSwapchainKHR swapchain) {
  uint32_t numImages;
  std::vector<VkImage> images;
  vkGetSwapchainImagesKHR(device, swapchain, &numImages, nullptr);
  images.resize(numImages);
  vkGetSwapchainImagesKHR(device, swapchain, &numImages, images.data());
  return images;
}

std::string physicalDeviceTypeToString(VkPhysicalDeviceType const type) {
  switch (type) {
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
      return "Integrated GPU";
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
      return "Discrete GPU";
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
      return "Virtual GPU";
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
      return "CPU";
    default:
      return "???";
  }
}

}