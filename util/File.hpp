//
// Created by kaede on 2019/09/27.
//

#pragma once

#include <optional>
#include <string>

namespace util {

std::string readAllFromFileAsString(std::string const &fileName) noexcept(false);
std::vector<uint8_t> readAllFromFile(std::string const &fileName) noexcept(false);

}
