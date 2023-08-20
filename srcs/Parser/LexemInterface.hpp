#pragma once

#include <common_inc.h>
#include <Server.hpp>

typedef std::string token;

// pure virtual class a.k.a. interface for Lexems
struct InterfaceLexem {
    virtual void addToServer(Server* serv) = 0;
    virtual void parseLexem(const std::vector<token> tokens, size_t& currentIndex) = 0;
    virtual ~InterfaceLexem() { }
};