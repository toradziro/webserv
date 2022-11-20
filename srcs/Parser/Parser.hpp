#pragma once

#include <common_inc.h>
#include <Config.hpp>

extern int confStringNum;

namespace Parser {
    Config* parseConfig(const std::string& confPath);
}
