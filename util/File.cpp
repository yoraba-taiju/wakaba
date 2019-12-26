//
// Created by kaede on 2019/09/27.
//

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
    std::error_code err = std::make_error_code(static_cast<std::errc>(errno));
    throw std::filesystem::filesystem_error("File not found", fileName, err);
  }
  std::uintmax_t const fileSize = std::filesystem::file_size(fileName);
  FILE *const file = fopen(fileName.c_str(), "rb");
  if (!file) {
    std::error_code err = std::make_error_code(static_cast<std::errc>(errno));
    throw std::filesystem::filesystem_error("Error to open file", fileName, err);
  }
  std::vector<uint8_t> dat;
  dat.resize(fileSize);
  size_t readed = fread(dat.data(), 1, fileSize, file);
  if (readed < fileSize) {
    std::error_code err = std::make_error_code(static_cast<std::errc>(errno));
    fclose(file);
    throw std::filesystem::filesystem_error("Error to read all contents from the file", fileName, err);
  }
  fclose(file);
  return std::move(dat);
}

}