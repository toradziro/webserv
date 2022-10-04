#pragma once

#include <common_inc.h>
#include <Server.hpp>
#include <Errors.hpp>

typedef std::string token;

// pure virtual class a.k.a. interface for Lexems
struct Lexem {
    virtual void addToServer(Server* serv) = 0;
    virtual void parseLexem(const std::vector<token> tokens, size_t& currentIndex) = 0;
    virtual ~Lexem() { }
};

class LocationLexem : public Lexem {
public:
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;
    ~LocationLexem() {}
private:
    // key - label, value - rootPath
    std::pair<std::string, std::string> location;
};

class ListenLexem : public Lexem {
public:
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;
    ~ListenLexem() {}
private:
    std::string ipAddress;
    uint16_t port;
};
