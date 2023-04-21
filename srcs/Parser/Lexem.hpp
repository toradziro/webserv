#pragma once

#include <common_inc.h>
#include <Errors.hpp>
#include <ConfigError.hpp>
#include <Parser.hpp>
#include <LexemInterface.hpp>

class LocationLexem : public InterfaceLexem {
public:
    LocationLexem(ConfigErrors* errors) : m_errors(errors) {}
    ~LocationLexem() {}
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;

private:
    // key - label, value - rootPath
    std::pair<std::string, std::string> m_location;

    ConfigErrors* m_errors;

    bool checkContainEnvVar(const std::string& rootPath);
    void readEnvVar(std::string& rootPath);
};

class ListenLexem : public InterfaceLexem {
public:
    ListenLexem(ConfigErrors* errors) : m_errors(errors) {}
    ~ListenLexem() {}
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;

private:
    std::string m_ipAddress;
    uint16_t    m_port;

    ConfigErrors* m_errors;
};

class ServerNameLexem : public InterfaceLexem {
public:
    ServerNameLexem(ConfigErrors* errors) : m_errors(errors) {}
    ~ServerNameLexem() {}
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;

private:
    std::string m_serverName;

    ConfigErrors* m_errors;
};

class ServerRootLexem : public InterfaceLexem {
public:
    ServerRootLexem(ConfigErrors* errors) : m_errors(errors) {}
    ~ServerRootLexem() {}
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;

private:
    std::string m_parsedPath;

    ConfigErrors* m_errors;
};

InterfaceLexem* createLexemByToken(const token& _token, ConfigErrors* errors);
