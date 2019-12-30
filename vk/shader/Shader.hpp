/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <memory>

namespace vk {
class Vulkan;
class ShaderModule;

/* protected class */
class Shader {
  friend class Vulkan;
private:
  std::weak_ptr<Vulkan> vulkan_;
  std::shared_ptr<ShaderModule> module_;
protected:
  template <typename T>
  static std::tuple<size_t, const uint32_t*> const shaderBianry;
public:
  Shader(Shader&&) = delete;
  Shader(Shader const&) = delete;
  Shader& operator=(Shader&&) = delete;
  Shader& operator=(Shader const&) = delete;

public:
  [[ nodiscard ]] std::shared_ptr<ShaderModule> const& module() { return this->module_; }

protected:
  explicit Shader(std::shared_ptr<Vulkan> const& vulkan, std::shared_ptr<ShaderModule> module)
  :vulkan_(vulkan)
  ,module_(std::move(module))
  {
  }
  virtual ~Shader() noexcept;
};

}