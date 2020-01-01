/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>
#include "../ShaderModule.hpp"

namespace vk {
class Vulkan;
class ShaderModule;

/* protected class */
class Shader {
  friend class Vulkan;
private:
  std::weak_ptr<Vulkan> vulkan_;
  ShaderModule module_;
protected:
  template <typename T>
  static std::tuple<size_t, const uint32_t*> loadBianry();
public:
  Shader(Shader&&) = delete;
  Shader(Shader const&) = delete;
  Shader& operator=(Shader&&) = delete;
  Shader& operator=(Shader const&) = delete;

public:
  [[ nodiscard ]] ShaderModule& module() { return this->module_; }

protected:
  explicit Shader(std::shared_ptr<Vulkan> const& vulkan, ShaderModule&& module)
  :vulkan_(vulkan)
  ,module_(std::move(module))
  {
  }
  virtual ~Shader() noexcept;
};

}