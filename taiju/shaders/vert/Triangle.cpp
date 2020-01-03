/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Triangle.hpp"

using taiju::shaders::vert::Triangle;
using vk::Shader;
using vk::VertexShader;

namespace taiju::shaders::vert {

Triangle::Triangle(std::shared_ptr<vk::Device> const& device, vk::ShaderModule&& module)
:vk::VertexShader(device, std::move(module))
{
}

}

namespace {
#include "taiju/shaders/vert/Triangle.vert.h"
}
template <>
std::tuple<size_t, const uint32_t*> Shader::loadBianry<Triangle>() {
  return std::tuple<size_t, const uint32_t*>(sizeof(shaderBinary), shaderBinary);
}

std::tuple<
    std::vector<VkVertexInputBindingDescription>,
    std::vector<VkVertexInputAttributeDescription>>
    Triangle::inputDescription()
{
  std::vector<VkVertexInputBindingDescription> bindings = {
      {
          .binding = 0,
          .stride = sizeof(Triangle::Input),
          .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
      },
  };
  std::vector<VkVertexInputAttributeDescription> attrs = {
      {
          .location = 0,
          .binding = 0,
          .format = VK_FORMAT_R32G32_SFLOAT,
          .offset = offsetof(Triangle::Input, pos)
      },
      {
          .location = 1,
          .binding = 0,
          .format = VK_FORMAT_R32G32B32_SFLOAT,
          .offset = offsetof(Triangle::Input, color)
      }
  };
  return std::make_tuple(std::move(bindings), std::move(attrs));
}