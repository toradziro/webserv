#include <Lexem.hpp>
#include <Errors.hpp>

#define DEFAULT_PORT 80

namespace Lexem {

// Fabric function
Lexem* createLexemByToken(const token& _token) {
    Lexem* lexem;
    if(_token == "location") {
        lexem = new LocationLexem();
    } else if(_token == "listen") {
        lexem = new ListenLexem();
    } else if(_token == "server_name") {
        lexem = new ServerNameLexem();
    } else {
        checkError(true, "wrong syntax: " + _token);
    }
    return lexem;
}

static void increaseIndex(size_t& currentIndex, size_t tokensSize) {
    ++currentIndex;
    checkError(currentIndex == tokensSize, "unclosed location server");
}

void LocationLexem::addToServer(Server* serv) {
    serv->addLocation(std::move(location.first), std::move(location.second));
}

void LocationLexem::parseLexem(const std::vector<token> tokens, size_t& currentIndex) {
    increaseIndex(currentIndex, tokens.size());
    std::string locationStr = tokens[currentIndex];
    std::string rootPath;
    increaseIndex(currentIndex, tokens.size());
    if(tokens[currentIndex] == "{") {
        increaseIndex(currentIndex, tokens.size());
        checkError(tokens[currentIndex] != "root", "missed root keyword in location");
        increaseIndex(currentIndex, tokens.size());
        rootPath = tokens[currentIndex];
        increaseIndex(currentIndex, tokens.size());
        checkError(tokens[currentIndex] != "}", "unclosed root section");
    } else {
        rootPath = tokens[currentIndex];
    }
    location.first = std::move(locationStr);
    location.second = std::move(rootPath);
}

void ListenLexem::addToServer(Server* serv) {
    serv->setIpAddress(std::move(ipAddress));
    serv->setListenPort(port);
}

void ListenLexem::parseLexem(const std::vector<token> tokens, size_t& currentIndex) {
    increaseIndex(currentIndex, tokens.size());
    size_t index = 0;
    bool needParsePort = false;
    for(; index < tokens[currentIndex].size(); ++index) {
        if(tokens[currentIndex][index] == ':') {
            needParsePort = true;
            break;
        }
        ipAddress += tokens[currentIndex][index];
    }
    if(needParsePort) {
        ++index;
        const char* tokenAsPtr = tokens[currentIndex].c_str();
        port = static_cast<uint16_t>(atoi(&(tokenAsPtr[index])));
    } else {
        port = DEFAULT_PORT;
    }
}

void ServerNameLexem::addToServer(Server* serv) {
    serv->setServerName(std::move(serverName));
}

void ServerNameLexem::parseLexem(const std::vector<token> tokens, size_t& currentIndex) {
    increaseIndex(currentIndex, tokens.size());
    serverName = std::move(tokens[currentIndex]);
}
} // namespace Lexem
