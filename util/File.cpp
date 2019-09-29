//
// Created by kaede on 2019/09/27.
//

#include <cstring>
#include <tinyformat.h>
#include <sys/stat.h>
#include <filesystem>
#include "File.hpp"

namespace util {

std::string readAllFromFile(std::string const& fileName) noexcept(false) {
  std::uintmax_t const fileSize = std::filesystem::file_size(fileName);
  FILE* const file = fopen(fileName.c_str(), "rb");
  if(!file) {
    std::error_code err = std::make_error_code(static_cast<std::errc>(errno));
    throw std::filesystem::filesystem_error("Error to open file", fileName, err);
  }
  std::string str;
  str.resize(fileSize);
  size_t readed = fread(str.data(), 1, fileSize, file);
  if(readed < fileSize) {
    std::error_code err = std::make_error_code(static_cast<std::errc>(errno));
    fclose(file);
    throw std::filesystem::filesystem_error("Error to read all contents from the file", fileName, err);
  }
  fclose(file);
  return str;
}

}