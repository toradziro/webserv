#pragma once

#include <common_inc.h>
#include <RequestHeader.hpp>
#include <EntityHeader.hpp>
#include <GeneralHeader.hpp>
#include <ContentTypeCollection.hpp>
#include <AllowedCGIExecutors.hpp>

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
    std::string             m_CGILocation;
    std::string             m_location;
    std::string             m_queryString;
    std::string             m_serverName;
    std::string             m_requestStringName;
    ContentTypeCollection*  m_contentTypes;
    AllowedCGIExecutors*    m_allowedCGI;
    RequestType             m_reqType;
    int                     m_clientFd;
};
