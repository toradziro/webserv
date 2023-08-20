#pragma once

#include <common_inc.h>

class ContentTypeCollection {
public:
    ContentTypeCollection() {}
    
    void addType(std::string& fileExtention, std::string& contentType);
    std::string getContentTypeByExtention(std::string fileExtention);

private:
    std::unordered_map<std::string, std::string> m_types;
};
