//
// Created by kaede on 2019/10/29.
//

#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

namespace vk {

std::vector<std::string> enumurateRequiredInstanceExtensions();
std::vector<VkLayerProperties> enumerateInstanceLayerProperties();

}


