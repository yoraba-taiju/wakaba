/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#include "Stream.hpp"
#include <fstream>

namespace donut {

Stream Stream::open(std::string filename) {
  // https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
  std::ifstream f(filename);
  std::string buff;
  f.seekg(0, std::ios::end);
  buff.reserve(f.tellg());
  f.seekg(0, std::ios::beg);
  buff.assign((std::istreambuf_iterator<char>(f)), (std::istreambuf_iterator<char>()));
  return Stream(std::move(filename), std::move(buff));
}

}
