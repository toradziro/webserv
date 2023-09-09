#include <Parser.hpp>
#include <Lexem.hpp>
#include <LexemsCollection.hpp>
#include <FileFuncs.hpp>
#include <ContentTypeParser.hpp>

int confStringNum = 1;

namespace Parser {

#define NOT_STRING 0

const std::vector<std::string> validLabels {
    "server",
    "listen",
    "location",
    "root",
    "server_name",
    "worker_threads",
    "server_root",
    "cgi_server_pass",
    "cgi_dir",
    "working_threads",
};

static bool isValidLabel(const token& _token) {
    for(size_t i = 0; i < validLabels.size(); ++i) {
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
        if(readFile[i] == '#') {
            while(i < len && readFile[i] != '\n') {
                ++i;
            }
        }
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
        if(readFile[i] == '\n') {
            tockens.push_back("\n");
        }
    }
    return tockens;
}

static void parseServer(LexemsCollection& lexems,
    const std::vector<token>& tokens, size_t& currentIndex, ConfigErrors* errors)
{
    if(tokens[currentIndex] == "{") {
        ++currentIndex;
    } else {
        errors->addError("lost curly bracket in server declaration", confStringNum);
    }
    for(;currentIndex < tokens.size(); ++currentIndex) {
        if(tokens[currentIndex] == "\n") {
            ++confStringNum;
        } else if(isValidLabel(tokens[currentIndex])) {
            InterfaceLexem* lexem = createLexemByToken(tokens[currentIndex], errors);
            if(lexem == nullptr) {
                return;
            }
            lexem->parseLexem(tokens, currentIndex);
            lexems.addLexem(lexem);
        } else if(tokens[currentIndex] == "}") {
            break;
        } else {
            errors->addError("unknown config syntax: " + tokens[currentIndex], confStringNum);
            return;
        }
    }
    if(tokens[currentIndex] != "}") {
        errors->addError("lost curly bracket in server declaration", confStringNum);
    }
}

static LexemsCollection* makeLexems(const std::vector<token>& tokens, ConfigErrors* errors) {
    LexemsCollection* lexems = new LexemsCollection;
    for(size_t i = 0; i < tokens.size(); ++i) {
        if(tokens[i] == "server") {
            ++i;
            parseServer(*lexems, tokens, i, errors);
        } else if(tokens[i] == "\n") {
            ++confStringNum;
        } else if(tokens[i] == "}") {
            break;
        } else {
            errors->addError("Config error in server section, possible cause is: " + tokens[i], confStringNum);
            return lexems;
        }
    }
    return lexems;
}

Config* parseConfig(const std::string& confPath) {
    ConfigErrors* errors = new ConfigErrors();
    int configFileFd = open(confPath.c_str(), O_RDONLY | O_EXCL);
    checkError(configFileFd == -1, "wasn't able to open config file: ");

    int fileSize = getFileSize(confPath);
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

    if(tokens.size() == 0) {
        errors->addError("empty config", NOT_STRING);
    }

    // Unmup file 'cause we don't need it anymore
    checkError(munmap((void*)configMapping, fileSize) == -1, "config unmapping failed");
    checkError(close(configFileFd) == -1, "config unmapping failed");

    LexemsCollection* lexems = makeLexems(tokens, errors);
    // Let's parse content-types allowed for http
    ContentTypeParser contentParser;
    contentParser.ParseContentType();

    Config* serverConfig = new Config(lexems, contentParser.getContentTypeCollection(), errors);
    return serverConfig;
}
}
