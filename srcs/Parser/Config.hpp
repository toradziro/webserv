#pragma once

#include <common_inc.h>
#include <LexemsCollection.hpp>

struct Config {
    explicit Config(LexemsCollection* lexems,
                    ContentTypeCollection* contentTypes) : 
                    m_lexems(lexems),
                    m_contentTypes(contentTypes) {}

    LexemsCollection*       m_lexems;
    ContentTypeCollection*  m_contentTypes;
};
