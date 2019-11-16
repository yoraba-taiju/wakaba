//
// Created by kaede on 2019/10/29.
//

#pragma once
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <vector>
#include <string>

namespace vk {

std::vector<std::string> enumerateRequiredInstanceExtensions();
std::vector<VkLayerProperties> enumerateInstanceLayerProperties();
std::vector<VkPhysicalDevice> enumeratePhysicalDevices(VkInstance instance);
std::vector<VkQueueFamilyProperties> getPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice);
std::vector<VkSurfaceFormatKHR> getPhysicalDeviceSurfaceFormats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
std::vector<VkPresentModeKHR> getPhysicalDeviceSurfacePresentModes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
std::vector<VkImage> getSwapchainImages(VkDevice device, VkSwapchainKHR swapchain);

}
