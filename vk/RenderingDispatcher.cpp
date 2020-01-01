/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "RenderingDispatcher.hpp"

namespace vk {

RenderingDispatcher::RenderingDispatcher(std::shared_ptr<Vulkan> vulkan)
:vulkan_(std::move(vulkan))
{

}

}