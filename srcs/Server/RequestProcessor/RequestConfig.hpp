#pragma once

#include <common_inc.h>
#include <RequestHeader.hpp>
#include <EntityHeader.hpp>
#include <GeneralHeader.hpp>
#include <ContentTypeCollection.hpp>

enum RequestType {
    RT_GET,
    RT_POST,
    RT_PUT,
    RT_UNKNOWN,
};

struct RequestConfig {
    GeneralHeader           generalHeaderTable;
    RequestHeader           requestHeaderTable;
    EntityHeader            entityHeaderTable;
    std::string             m_body;
    std::string             m_serverRoot;
    std::string             m_location;
    ContentTypeCollection*  m_contentTypes;
    RequestType             m_reqType;
    int                     m_clientFd;
};
