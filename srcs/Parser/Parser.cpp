#include <Parser.hpp>
#include <Lexem.hpp>
#include <LexemsCollection.hpp>
#include <FileFuncs.hpp>

namespace Parser {

std::string validLabels[] {
    "server",
    "listen",
    "location",
    "root",
    "server_name",
    "worker_threads"
};

static bool isValidLabel(const token& _token) {
    for(size_t i = 0; i < validLabels->size(); ++i) {
        if(_token == validLabels[i]) {
            return true;
        }
    }
    return false;
}

static std::vector<token> makeTokens(const char* readFile) {
    size_t len = strlen(readFile);
    std::vector<token> tockens;

    for(size_t i = 0; i < len; ++i) {
        token localTocken = "";
        bool addLexem = false;
        while(i < len &&
            readFile[i] != ' ' &&
            readFile[i] != '\t' &&
            readFile[i] != '\n')
        {
            addLexem = true;
            localTocken += readFile[i];
            ++i;
        }
        if(addLexem == true)
            tockens.push_back(std::move(localTocken));
    }
    return std::move(tockens);
}

static void parseServer(LexemsCollection& lexems,
    const std::vector<token>& tokens, size_t& currentIndex)
{
    if(tokens[currentIndex] == "{") {
        ++currentIndex;
    } else {
        checkError(true, "lost curly bracket in server declaration");
    }
    for(;currentIndex < tokens.size(); ++currentIndex) {
        if(isValidLabel(tokens[currentIndex])) {
            Lexem::InterfaceLexem* lexem = Lexem::createLexemByToken(tokens[currentIndex]);
            lexem->parseLexem(tokens, currentIndex);
            lexems.addLexem(lexem);
        } else if(tokens[currentIndex] == "}") {
            break;
        } else {
            checkError(true, "wrong syntax: " + tokens[currentIndex]);
        }
    }
    if(tokens[currentIndex] != "}") {
        checkError(true, "lost curly bracket in server closing");
    }
}

static LexemsCollection makeLexems(const std::vector<token>& tokens) {
    LexemsCollection lexems;
    for(size_t i = 0; i < tokens.size(); ++i) {
        if(tokens[i] == "server") {
            ++i;
            parseServer(lexems, tokens, i);
        } else if(tokens[i] == "}") {
            break;
        } else {
            checkError(true, "unknown config syntax: " + tokens[i]);
        }
    }
    return std::move(lexems);
}

Server* parseConfig(const std::string& confPath) {
    Server* serv = new Server;
    int configFileFd = open(confPath.c_str(), O_RDONLY | O_EXCL);
    
    checkError(configFileFd == -1, "wasn't able to open config file: ");

    int fileSize = getFileSize(configFileFd);
    checkError(fileSize == -1, "wasn't able to get config size");
    // Map config file in memory
    char* configMapping = (char*)mmap(NULL,
                            fileSize,
                            PROT_READ,
                            MAP_PRIVATE,
                            configFileFd, 0);
    checkError(configMapping == NULL, "config mapping failed");

    // Make tockens from config
    std::vector<token> tokens = makeTokens(configMapping);
    checkError(tokens.size() == 0, "empty config");

    // Unmup file 'cause we don't need it anymore
    checkError(munmap((void*)configMapping, fileSize) == -1, "config unmapping failed");

    LexemsCollection lexems = makeLexems(tokens);
    lexems.addToServer(serv);
    serv->checkServerInstance();
    return serv;
}
}