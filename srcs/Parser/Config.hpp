#pragma once

#include <common_inc.h>
#include <LexemsCollection.hpp>
#include <ConfigError.hpp>

struct Config {
    explicit Config(LexemsCollection* lexems,
                    ContentTypeCollection* contentTypes,
                    ConfigErrors* errors) : 
                    m_lexems(lexems),
                    m_contentTypes(contentTypes),
                    m_errors(errors) {}

    ~Config() {
        if(m_errors != nullptr) {
            delete m_errors;
        }
    }

    bool hasError() {
        if(m_errors->hasErrors()) {
            return true;
        }
        return false;
    }

    void validateConfig() {
        if(hasError()) {
            m_errors->printErrors();
        }
        delete m_lexems;
        m_lexems = nullptr;
        delete m_contentTypes;
        m_contentTypes = nullptr;
        delete m_errors;
        m_errors = nullptr;
        exit(EXIT_FAILURE);
    }

    LexemsCollection*       m_lexems;
    ContentTypeCollection*  m_contentTypes;
    ConfigErrors*           m_errors;
};
