/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include "Triangle.hpp"

using taiju::shaders::Triangle;
using vk::Shader;

namespace {
namespace frag {
#include "taiju/shaders/Triangle.frag.h"
}
namespace vert {
#include "taiju/shaders/Triangle.vert.h"
}
}

namespace taiju::shaders {

Triangle::Frag::Frag(std::shared_ptr<vk::Device> const& device, vk::ShaderModule&& module)
:vk::FragmentShader(device, std::move(module))
{
}

}

template <>
std::tuple<size_t, const uint32_t*> Shader::loadBianry<Triangle::Frag>() {
  return std::tuple<size_t, const uint32_t*>(sizeof(frag::shaderBinary), frag::shaderBinary);
}

Triangle::Vert::Vert(std::shared_ptr<vk::Device> const& device, vk::ShaderModule&& module)
:vk::VertexShader(device, std::move(module))
{
}

template <>
std::tuple<size_t, const uint32_t*> vk::Shader::loadBianry<Triangle::Vert>() {
  return std::tuple<size_t, const uint32_t*>(sizeof(vert::shaderBinary), vert::shaderBinary);
}

std::tuple<
    std::vector<VkVertexInputBindingDescription>,
    std::vector<VkVertexInputAttributeDescription>>
Triangle::Vert::inputDescription()
{
  std::vector<VkVertexInputBindingDescription> bindings = {
      {
          .binding = 0,
          .stride = sizeof(Input),
          .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
      },
  };
  std::vector<VkVertexInputAttributeDescription> attrs = {
      {
          .location = 0,
          .binding = 0,
          .format = VK_FORMAT_R32G32_SFLOAT,
          .offset = offsetof(Input, pos)
      },
      {
          .location = 1,
          .binding = 0,
          .format = VK_FORMAT_R32G32B32_SFLOAT,
          .offset = offsetof(Input, color)
      }
  };
  return std::make_tuple(std::move(bindings), std::move(attrs));
}