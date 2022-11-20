#include <ConfigError.hpp>

ConfigErrors::ConfigErrors() : m_errors() { }

ConfigErrors::~ConfigErrors() {
    for(size_t i = 0; i < m_errors.size(); ++i) {
        delete m_errors[i];
    }
}

bool ConfigErrors::hasErrors() {
    if(m_errors.size() == 0) {
        return false;
    }
    return true;
}

void ConfigErrors::addError(const std::string& description, int stringNum) {
    ConfigError* error = new ConfigError(description, stringNum);
    m_errors.push_back(error);
}

void ConfigErrors::printErrors() {
    for(size_t i = 0; i < m_errors.size(); ++i) {
        if(m_errors[i]->m_stringNum > 0) {
            std::cerr << ANSI_COLOR_RED << "Config error in str: " << ANSI_COLOR_RESET << m_errors[i]->m_stringNum << std::endl;
        }
        std::cerr << m_errors[i]->m_desctiprion << std::endl;
    }
}
