#include <ContentTypeCollection.hpp>

void ContentTypeCollection::addType(std::string& fileExtention, std::string& contentType) {
    m_types[fileExtention] = contentType;
}

std::string ContentTypeCollection::getContentTypeByExtention(std::string fileExtention) {
    if(m_types.count(fileExtention) != 0) {
        return m_types[fileExtention];
    }
    return "";
}
