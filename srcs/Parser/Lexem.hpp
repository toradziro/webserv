#pragma once

#include <common_inc.h>
#include <Server.hpp>
#include <Errors.hpp>

typedef std::string token;

namespace Lexem {

// pure virtual class a.k.a. interface for Lexems
struct InterfaceLexem {
    virtual void addToServer(Server* serv) = 0;
    virtual void parseLexem(const std::vector<token> tokens, size_t& currentIndex) = 0;
    virtual ~InterfaceLexem() { }
};

class LocationLexem : public InterfaceLexem {
public:
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;
    ~LocationLexem() {}

private:
    // key - label, value - rootPath
    std::pair<std::string, std::string> m_location;

    bool checkContainEnvVar(const std::string& rootPath);
    void readEnvVar(std::string& rootPath);
};

class ListenLexem : public InterfaceLexem {
public:
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;
    ~ListenLexem() {}

private:
    std::string m_ipAddress;
    uint16_t    m_port;
};

class ServerNameLexem : public InterfaceLexem {
public:
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;
    ~ServerNameLexem() {}

private:
    std::string m_serverName;
};

InterfaceLexem* createLexemByToken(const token& _token);

} // namespace Lexem
