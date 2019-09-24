#pragma once

#include <cstdio>
#include <ctime>
#include <iomanip>
#include <tinyformat.h>

namespace util {

class Logger final {
public:
  enum Level : int {
    TRACE = 0,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
  };

public:
  explicit Logger(FILE *const output)
      : output_(output), level_(Level::WARN) {
  }

  Logger() : Logger(stdout) {
  }

  template<typename ...Args>
  void trace(std::string const &fmt, Args &&...args) {
    this->log_(Level::TRACE, fmt, std::forward<Args>(args)...);
  }

  template<typename ...Args>
  void debug(std::string const &fmt, Args &&...args) {
    this->log_(Level::DEBUG, fmt, std::forward<Args>(args)...);
  }

  template<typename ...Args>
  void info(std::string const &fmt, Args &&...args) {
    this->log_(Level::INFO, fmt, std::forward<Args>(args)...);
  }

  template<typename ...Args>
  void error(std::string const &fmt, Args &&...args) {
    this->log_(Level::ERROR, fmt, std::forward<Args>(args)...);
  }

  template<typename ...Args>
  void fatal(std::string const &fmt, Args &&...args) {
    this->log_(Level::FATAL, fmt, std::forward<Args>(args)...);
  }

  void setLevel(const Level level) {
    this->level_ = level;
  }

private:
  template<typename ...Args>
  void log_(Level const level, std::string const &fmt, Args &&...args) {
    if (level_ > level) {
      return;
    }
    std::string const msg = tfm::format(fmt.c_str(), std::forward<Args>(args)...);
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::stringstream ss;
    ss  << std::put_time(&tm, "%Y/%m/%d% %H:%M:%S");
    std::string const time = ss.str();
    switch (level) {
      case Level::TRACE:
        fprintf(this->output_, "[%s TRACE] %s\n", time.c_str(), msg.c_str());
        break;
      case Level::DEBUG:
        fprintf(this->output_, "[%s DEBUG] %s\n", time.c_str(), msg.c_str());
        break;
      case Level::INFO:
        fprintf(this->output_, "[%s INFO ] %s\n", time.c_str(), msg.c_str());
        break;
      case Level::WARN:
        fprintf(this->output_, "[%s WARN ] %s\n", time.c_str(), msg.c_str());
        break;
      case Level::ERROR:
        fprintf(this->output_, "[%s ERROR] %s\n", time.c_str(), msg.c_str());
        break;
      case Level::FATAL:
        throw std::runtime_error("[FATAL] " + msg);
    }
    fflush(this->output_);
  }

private:
  FILE *const output_;
  Level level_;
};
}
