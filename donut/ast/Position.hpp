/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */
#pragma once

#include <string>

namespace donut::ast {

class Position final {
public:
  inline Position(size_t line, size_t column) noexcept
  :line_(line)
  ,column_(column){

  }
  Position() = delete;

public:
  [[nodiscard]] size_t line() const { return this->line_; }
  [[nodiscard]] size_t column() const { return this->column_; }
private:
  size_t const line_;
  size_t const column_;
};

class Range final {
public:
  inline Range(std::string&& filename, Position&& beg, Position&& end)
  :filename_(std::forward<std::string>(filename))
  ,begin_(std::forward<Position>(beg))
  ,end_(std::forward<Position>(end))
  {

  }
  Range() = delete;

public:
  [[nodiscard]] std::string const& filename() const { return this->filename_; }
  [[nodiscard]] Position const& begin() const { return this->begin_; }
  [[nodiscard]] Position const& end() const { return this->end_; }
private:
  std::string const filename_;
  Position const begin_;
  Position const end_;
};

}
