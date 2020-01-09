/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <cstdio>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fmt/format.h>
#include <chrono>

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
  Logger(Logger const&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger const&) = delete;
  Logger& operator=(Logger&&) = delete;

  explicit Logger(FILE *const outputStandard, FILE *const outputError)
  :outputStandard_(outputStandard)
  ,outputError_(outputError)
  ,level_(Level::WARN) {
  }

  Logger() : Logger(stdout, stderr) {
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
  void warn(std::string const &fmt, Args &&...args) {
    this->log_(Level::WARN, fmt, std::forward<Args>(args)...);
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
    std::string const msg = fmt::format(fmt, std::forward<Args>(args)...);
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &t);
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S");
    std::string const time = ss.str();
    switch (level) {
      case Level::TRACE:
        fprintf(this->outputStandard_, "[%s TRACE] %s\n", time.c_str(), msg.c_str());
        fflush(this->outputStandard_);
        break;
      case Level::DEBUG:
        fprintf(this->outputStandard_, "[%s DEBUG] %s\n", time.c_str(), msg.c_str());
        fflush(this->outputStandard_);
        break;
      case Level::INFO:
        fprintf(this->outputStandard_, "[%s INFO ] %s\n", time.c_str(), msg.c_str());
        fflush(this->outputStandard_);
        break;
      case Level::WARN:
        fprintf(this->outputError_, "[%s WARN ] %s\n", time.c_str(), msg.c_str());
        fflush(this->outputError_);
        break;
      case Level::ERROR:
        fprintf(this->outputError_, "[%s ERROR] %s\n", time.c_str(), msg.c_str());
        fflush(this->outputError_);
        break;
      case Level::FATAL:
        throw std::runtime_error("[FATAL] " + msg);
    }
  }

private:
  FILE *const outputStandard_;
  FILE *const outputError_;
  Level level_;
};
}
