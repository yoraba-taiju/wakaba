/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#include <cstring>
#include <vector>
#include <filesystem>
#include "File.hpp"

namespace util {

std::string readAllFromFileAsString(std::string const &fileName) noexcept(false) {
  std::vector<uint8_t> dat = readAllFromFile(fileName);
  return std::string(dat.begin(), dat.end());
}

std::vector<uint8_t> readAllFromFile(std::string const &fileName) noexcept(false) {
  if (!std::filesystem::exists(fileName)) {
    std::error_code err = std::make_error_code(static_cast<std::errc>(ENOENT));
    throw std::filesystem::filesystem_error("File not found", fileName, err);
  }
  size_t const fileSize = static_cast<size_t>(std::filesystem::file_size(fileName));
  FILE* file = nullptr;
  errno_t  err = fopen_s(&file, fileName.c_str(), "rb");
  if (err) {
    std::error_code code = std::make_error_code(static_cast<std::errc>(err));
    throw std::filesystem::filesystem_error("Error to open file", fileName, code);
  }
  std::vector<uint8_t> dat;
  dat.resize(fileSize);
  size_t pos = 0;
  while(!std::feof(file)) {
    size_t const left = dat.size() - pos;
    size_t const readed = fread_s(std::next(dat.data(), pos), dat.size(), 1, left, file);
    if (readed < left && ferror(file) != 0) {
      std::error_code err = std::make_error_code(static_cast<std::errc>(ferror(file)));
      fclose(file);
      throw std::filesystem::filesystem_error("Error to read all contents from the file", fileName, err);
    }
    pos += readed;
  }
  fclose(file);
  return std::move(dat);
}

}