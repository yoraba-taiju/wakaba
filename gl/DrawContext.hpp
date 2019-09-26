//
// Created by kaede on 2019/09/26.
//

#pragma once

#include <memory>
#include <unordered_map>
#include "../util/SharedUtil.hpp"

namespace gl {

class Program;
class ArrayBuffer;
class IndexBuffer;

class DrawContext final : public std::enable_shared_from_this<DrawContext> {
private:
  std::shared_ptr<Program> program_;
  std::unordered_map<int, std::shared_ptr<ArrayBuffer>> buffers_;
  std::shared_ptr<IndexBuffer> indicies_;
public:
  DrawContext(std::shared_ptr<Program> program);

  ~DrawContext() = default;

  void draw();

  void attach(std::string const &attrName, std::shared_ptr<ArrayBuffer> buffer);

  void attach(std::shared_ptr<IndexBuffer> indicies);

  template<typename... Args>
  static std::shared_ptr<DrawContext> create(Args &&...arg) {
    return util::make_shared<DrawContext>(std::forward<Args>(arg)...);
  }

public:
  ENABLE_SHARED_HELPER
};
}
