#include <Lexem.hpp>
#include <Errors.hpp>

#define DEFAULT_PORT 80

namespace Lexem {

// Fabric function
InterfaceLexem* createLexemByToken(const token& _token) {
    InterfaceLexem* lexem = nullptr;
    if(_token == "location") {
        lexem = new LocationLexem();
    } else if(_token == "listen") {
        lexem = new ListenLexem();
    } else if(_token == "server_name") {
        lexem = new ServerNameLexem();
    }
    return lexem;
}

static void increaseIndex(size_t& currentIndex, size_t tokensSize) {
    ++currentIndex;
    checkError(currentIndex == tokensSize, "unclosed location server");
}

void LocationLexem::addToServer(Server* serv) {
    serv->addLocation(std::move(m_location.first), std::move(m_location.second));
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
        if(checkContainEnvVar(rootPath)) {
            readEnvVar(rootPath);
        }
        increaseIndex(currentIndex, tokens.size());
        checkError(tokens[currentIndex] != "}", "unclosed root section");
    } else {
        rootPath = tokens[currentIndex];
    }
    m_location.first = std::move(locationStr);
    m_location.second = std::move(rootPath);
}

bool LocationLexem::checkContainEnvVar(const std::string& rootPath) {
    for(size_t i = 0; i < rootPath.size(); ++i) {
        if(rootPath[i] == '$') {
            return true;
        }
    }
    return false;
}

void LocationLexem::readEnvVar(std::string& rootPath) {
    std::string beforeEnv;
    std::string envName;
    std::string afterEnv;
    size_t index = 0;
    for(; index < rootPath.size() && rootPath[index] != '$'; ++index) {
        beforeEnv += rootPath[index];
    }
    ++index;
    for(; index < rootPath.size() && rootPath[index] != '/'; ++index) {
        envName += rootPath[index];
    }
    for(; index < rootPath.size(); ++index) {
        afterEnv += rootPath[index];
    }
    char* envValue = getenv(envName.c_str());
    rootPath = std::move(beforeEnv + std::string(envValue) + afterEnv);
}

void ListenLexem::addToServer(Server* serv) {
    serv->setIpAddress(std::move(m_ipAddress));
    serv->setListenPort(m_port);
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
        m_ipAddress += tokens[currentIndex][index];
    }
    if(needParsePort) {
        ++index;
        const char* tokenAsPtr = tokens[currentIndex].c_str();
        m_port = static_cast<uint16_t>(atoi(&(tokenAsPtr[index])));
    } else {
        m_port = DEFAULT_PORT;
    }
}

void ServerNameLexem::addToServer(Server* serv) {
    serv->setServerName(std::move(m_serverName));
}

void ServerNameLexem::parseLexem(const std::vector<token> tokens, size_t& currentIndex) {
    increaseIndex(currentIndex, tokens.size());
    m_serverName = std::move(tokens[currentIndex]);
}
} // namespace Lexem
