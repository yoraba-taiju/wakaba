
//
// Created by kaede on 2019/12/29.
//

#include "Triangle.hpp"

using taiju::shaders::vert::Triangle;
using vk::Shader;

namespace taiju::shaders::vert {

Triangle::Triangle(std::shared_ptr<vk::Vulkan> const& vulkan, std::shared_ptr<vk::ShaderModule> module)
:vk::VertexShader(vulkan, std::move(module))
{
}

}

namespace {
#include "taiju/shaders/vert/Triangle.vert.h"
}
template <>
std::tuple<size_t, const uint32_t*> Shader::shaderBianry<Triangle> = std::tuple<size_t, const uint32_t*>(sizeof(shaderBinary), shaderBinary);
