#pragma once

#include <cstdio>

namespace taiju{
  class Logger final {
  public:
    enum class Level : int {
      TRACE = 0,
      DEBUG,
      WARN,
      ERROR,
      FATAL,
    };

  public:
    Logger(FILE * const output)
    :output_(output)
    ,level_(Level::WARN) {
    }
    Logger():Logger(stdout){
    }
  private:
    FILE* const output_;
    Level level_;
  };
}
