/* coding: utf-8 */
/**
 * YorabaTaiju/Wakaba
 *
 * Copyright 2019-, Kaede Fujisaki
 */

#pragma once

#include <optional>
#include <string>

namespace util {

std::string readAllFromFileAsString(std::string const &fileName) noexcept(false);
std::vector<uint8_t> readAllFromFile(std::string const &fileName) noexcept(false);

}
