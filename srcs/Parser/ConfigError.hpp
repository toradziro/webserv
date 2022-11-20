#pragma once

#include <common_inc.h>

struct ConfigError {
public:
    ConfigError(const std::string& description, int stringNum) :
                m_desctiprion(description), m_stringNum(stringNum) {}

    std::string m_desctiprion;
    int         m_stringNum;
};

class ConfigErrors {
public:
    ConfigErrors();
    ~ConfigErrors();

    bool hasErrors();
    void addError(const std::string& description, int stringNum);
    void printErrors();
private:
    std::vector<ConfigError*> m_errors;
};
