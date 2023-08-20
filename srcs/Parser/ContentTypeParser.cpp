#include <ContentTypeParser.hpp>
#include <Errors.hpp>

const char* contentTypesConfig = "./available_data_types/content_types.conf";

ContentTypeParser::ContentTypeParser() {
    m_typesCollection = new ContentTypeCollection();
}

ContentTypeCollection* ContentTypeParser::getContentTypeCollection() {
    return m_typesCollection;
}

static std::string getWord(char* line, size_t& index) {
    size_t length = strlen(line);
    std::string result = "";
    for(;index < length; ++index) {
        if(line[index] == '\t' || line[index] == ' ') {
            break;
        }
        result += line[index];
    }
    return result;
}

static void skipWhiteSpaces(char* line, size_t& index) {
    size_t length = strlen(line);
    while(index < length && isalpha(line[index]) == 0) {
        ++index;
    }
}

void ContentTypeParser::ParseContentType() {
    FILE* file = fopen(contentTypesConfig, "r");
    checkError(file == NULL, "can't open content-types config");
    char* line = NULL;
    while(getline(&line, 0, file) != EOF) {
        size_t index = 0;
        std::string contentType = getWord(line, index);
        skipWhiteSpaces(line, index);
        std::string extention = getWord(line, index);
        memset(line, 0, strlen(line));
        m_typesCollection->addType(extention, contentType);
        free(line);
        line = NULL;
    }
    free(line);
    fclose(file);
}
