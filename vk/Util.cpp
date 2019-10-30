//
// Created by kaede on 2019/10/29.
//

#include "Util.hpp"

namespace vk {

std::vector<std::string> enumurateRequiredInstanceExtensions() {
  uint32_t numExtensions;
  const char **const extensions = glfwGetRequiredInstanceExtensions(&numExtensions);
  std::vector<std::string> res;
  res.reserve(numExtensions);
  for (uint32_t i = 0; i < numExtensions; ++i) {
    res.emplace_back(extensions[i]);
  }
  return std::move(res);
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
  return std::move(props);
}

}