//
// Created by kaede on 2019/12/01.
//

#pragma once

#include <memory>
#include <string>
#include <vulkan/vulkan.h>
#include "../util/Shared.hpp"

namespace vk{
class Vulkan;

class ShaderModule final {
private:
  friend class Vulkan;
  VkShaderModule obj_;
public:
  ENABLE_SHARED_HELPER
};

}
