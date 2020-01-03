/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

namespace vk{
class Vulkan;

class Bridge {
private:
  std::weak_ptr<Vulkan> vulkan_;
public:

};

}
