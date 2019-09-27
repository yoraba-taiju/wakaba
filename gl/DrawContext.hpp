//
// Created by kaede on 2019/09/26.
//

#pragma once

#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>
#include "../util/Shared.hpp"
#include "vertex/VertexArray.hpp"

namespace gl {

class Program;
class ArrayBuffer;
class IndexBuffer;

class DrawContext final : public std::enable_shared_from_this<DrawContext> {
private:
  std::shared_ptr<VertexArray> vertexArray_;
  std::shared_ptr<Program> program_;
  std::unordered_map<int, std::shared_ptr<ArrayBuffer>> buffers_;
  std::shared_ptr<IndexBuffer> indices_;
public:
  explicit DrawContext(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<Program> program);

  ~DrawContext() = default;

  void draw();

  void attach(std::string const &attrName, std::shared_ptr<ArrayBuffer> buffer);
  void attach(std::shared_ptr<IndexBuffer> indices);

  static std::shared_ptr<DrawContext> create(std::shared_ptr<Program> const& program) {
    return util::make_shared<DrawContext>(VertexArray::create(), program);
  }

public:
  ENABLE_SHARED_HELPER
};
}
