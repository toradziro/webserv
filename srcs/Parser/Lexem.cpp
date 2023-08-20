#include <Lexem.hpp>
#include <FileFuncs.hpp>

#define DEFAULT_PORT 80

// Fabric function
InterfaceLexem* createLexemByToken(const token& _token, ConfigErrors* errors) {
    InterfaceLexem* lexem = nullptr;
    if(_token == "location") {
        lexem = new LocationLexem(errors);
    } else if(_token == "listen") {
        lexem = new ListenLexem(errors);
    } else if(_token == "server_name") {
        lexem = new ServerNameLexem(errors);
    } else if(_token == "server_root") {
        lexem = new ServerRootLexem(errors);
    }
    return lexem;
}

static void increaseIndex(size_t& currentIndex, size_t tokensSize, const std::vector<token> tokens, ConfigErrors* errors) {
    ++currentIndex;
    while(tokens[currentIndex] == "\n") {
        ++currentIndex;
        ++confStringNum;
    }
    if(currentIndex == tokensSize) {
        errors->addError("unclosed server", confStringNum);
    }
}

void LocationLexem::addToServer(Server* serv) {
    serv->addLocation(std::move(m_location.first), std::move(m_location.second));
}

void LocationLexem::parseLexem(const std::vector<token> tokens, size_t& currentIndex) {
    increaseIndex(currentIndex, tokens.size(), tokens, m_errors);
    std::string locationStr = tokens[currentIndex];
    std::string rootPath;
    increaseIndex(currentIndex, tokens.size(), tokens, m_errors);
    if(tokens[currentIndex] == "{") {
        increaseIndex(currentIndex, tokens.size(), tokens, m_errors);
        if(tokens[currentIndex] != "root") {
            m_errors->addError("missed root keyword in location", confStringNum);
        }
        increaseIndex(currentIndex, tokens.size(), tokens, m_errors);
        rootPath = tokens[currentIndex];
        if(checkContainEnvVar(rootPath)) {
            readEnvVar(rootPath);
        }
        increaseIndex(currentIndex, tokens.size(), tokens, m_errors);
        if(tokens[currentIndex] != "}") {
            m_errors->addError("unclosed root section", confStringNum);
        }
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
    if(envValue != NULL) {
        rootPath = std::move(beforeEnv + std::string(envValue) + afterEnv);
    } else {
        m_errors->addError("Env value is used, but not set: ", confStringNum);
        rootPath = std::move(beforeEnv + afterEnv);
    }
}

void ListenLexem::addToServer(Server* serv) {
    serv->setIpAddress(std::move(m_ipAddress));
    serv->setListenPort(m_port);
}

void ListenLexem::parseLexem(const std::vector<token> tokens, size_t& currentIndex) {
    increaseIndex(currentIndex, tokens.size(), tokens, m_errors);
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
    increaseIndex(currentIndex, tokens.size(), tokens, m_errors);
    m_serverName = std::move(tokens[currentIndex]);
}

void ServerRootLexem::parseLexem(const std::vector<token> tokens, size_t& currentIndex) {
    increaseIndex(currentIndex, tokens.size(), tokens, m_errors);
    int index = 0;
    int length = tokens[currentIndex].size();
    if(tokens[currentIndex][0] == '"') {
        ++index;
        --length;
    }
    while(index < length) {
        m_parsedPath += tokens[currentIndex][index];
        ++index;
    }
    if(m_parsedPath == "") {
        m_parsedPath = GetCurrentDirectory();
    }
}

void ServerRootLexem::addToServer(Server* serv) {
    serv->setServerRoot(m_parsedPath);
}
