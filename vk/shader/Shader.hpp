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
class Device;
class ShaderModule;

/* protected class */
class Shader {
  friend class Device;
private:
  std::shared_ptr<Device> device_;
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
  explicit Shader(std::shared_ptr<Device> device, ShaderModule&& module)
  :device_(std::move(device))
  ,module_(std::move(module))
  {
  }
  virtual ~Shader() noexcept;
};

}