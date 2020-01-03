/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Triangle.hpp"

using taiju::shaders::frag::Triangle;
using vk::Shader;

namespace taiju::shaders::frag {

Triangle::Triangle(std::shared_ptr<vk::Device> const& device, vk::ShaderModule&& module)
:vk::FragmentShader(device, std::move(module))
{
}

}

namespace {
#include "taiju/shaders/frag/Triangle.frag.h"
}
template <>
std::tuple<size_t, const uint32_t*> Shader::loadBianry<Triangle>() {
  return std::tuple<size_t, const uint32_t*>(sizeof(shaderBinary), shaderBinary);
}