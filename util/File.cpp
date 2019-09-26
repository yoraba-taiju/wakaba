//
// Created by kaede on 2019/09/27.
//

#include <fstream>
#include "File.hpp"

namespace util {

std::string readAllFromFile(std::string const& fileName) noexcept(false) {
  std::ifstream fs(fileName);
  
  try {
    fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  } catch (std::system_error& e) {
    throw e;
  }

  std::string str;

  fs.seekg(0, std::ios::end);
  str.reserve(fs.tellg());
  fs.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(fs)),
             std::istreambuf_iterator<char>());
  return str;
}

}