#pragma once

#include <common_inc.h>
#include <Server.hpp>

// pure virtual class for for polymorphic work of lexems
struct Lexem {
    virtual void addToServer(Server* serv) = 0;
    virtual void parseLexem(const char* confPtr) = 0;
};

class LocationLexem : public Lexem {
public:
    void parseLexem(const char* confPtr) override {

    }
    void addToServer(Server* serv) override {

    }
private:
    std::pair<std::string, std::string> location;
};

class ListenLexem : public Lexem {
public:
    void parseLexem(const char* confPtr) override {

    }
    void addToServer(Server* serv) override {
        
    }
private:
    std::string ipAddress;
    uint16_t port;
};