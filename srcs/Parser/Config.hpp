#pragma once

#include <common_inc.h>
#include <LexemsCollection.hpp>

struct Config {
    explicit Config(LexemsCollection lexems) : m_lexems(lexems) {}
    LexemsCollection m_lexems;
};
