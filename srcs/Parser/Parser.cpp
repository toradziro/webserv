#include <Parser.hpp>

namespace Parser {

/*
Итоговая конфигурация блока server должна выглядеть следующим образом:

server {
    location / {
        root /data/www;
    }

    location /images/ {
        root /data;
    }
}
Это уже работающая конфигурация сервера, слушающего на стандартном порту 80 и доступного на локальном компьютере по адресу http://localhost/. 
В ответ на запросы, URI которых начинаются с /images/, сервер будет отправлять файлы из каталога /data/images. Например, 
на запрос http://localhost/images/example.png nginx отправит в ответ файл /data/images/example.png. 
Если же этот файл не существует, nginx отправит ответ, указывающий на ошибку 404. 
Запросы, URI которых не начинаются на /images/, будут отображены на каталог /data/www. 
Например, в результате запроса http://localhost/some/example.html в ответ будет отправлен файл /data/www/some/example.html.
*/

// Valid sintax key-words for config file
std::string validLabels[] {
    "server",
    "listen",
    "location",
    "root"
};

static int getFileSize(int fd) {
    struct stat st;

    if(fstat(fd, &st) == -1)
        return -1;
    return st.st_size;
}

/* Will need for lexer
static bool checkValidLabel(std::string label) {
    for(const auto& it : validLabels) {
        if(it == label) {
            return true;
        }
    }
    return false;
}
*/

static std::vector<std::string> makeTockens(const char* readFile) {
    // TODO: Change to move semantic

    size_t len = strlen(readFile);
    std::vector<std::string> tockens;

    for(size_t i = 0; i < len; ++i) {
        std::string lexem = "";
        bool addLexem = false;
        while(i < len &&
            readFile[i] != ' ' &&
            readFile[i] != '\t' &&
            readFile[i] != '\n')
        {
            addLexem = true;
            lexem += readFile[i];
            ++i;
        }
        if(addLexem == true)
            tockens.push_back(lexem);
    }
    return tockens;
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
    std::vector<std::string> tockens = makeTockens(configMapping);
    checkError(tockens.size() == 0, "empty config");
    // Unmup file 'cause we don't need it anymore
    checkError(munmap((void*)configMapping, fileSize) == -1, "config unmapping failed");
    
    for(size_t i = 0; i < tockens.size(); ++i) {
        std::cout << tockens[i] << std::endl;
    }

    return serv;
}
}