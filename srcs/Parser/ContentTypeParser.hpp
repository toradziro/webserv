#pragma once

#include <common_inc.h>
#include <ContentTypeCollection.hpp>

class ContentTypeParser {
public:
    ContentTypeParser();
    ContentTypeCollection* getContentTypeCollection();
    void ParseContentType();

private:
    ContentTypeCollection* m_typesCollection;
};