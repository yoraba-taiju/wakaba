//
// Created by kaede on 2019/09/26.
//

#pragma once

#include <memory>
#include <unordered_map>

namespace gl {

class Program;
class Buffer;

class RenderContext final : public std::enable_shared_from_this<RenderContext> {
private:
  std::shared_ptr<Program> program_;
  std::unordered_map<unsigned int, std::shared_ptr<Buffer>> buffers_;
public:
  
};

}
