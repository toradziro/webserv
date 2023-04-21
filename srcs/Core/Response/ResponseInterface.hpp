#pragma once

#include <common_inc.h>
#include <ContentTypeCollection.hpp>

enum ResponseType {
    RESP_GET,
    RESP_POST,

    RESP_UNKNOWN,
};

enum ResponseCode {
    RC_200,
    RC_404,

    RC_LENGTH,
};

class ResponseInterface {
public:
    virtual ~ResponseInterface() { }
    virtual void sendResponse() = 0;
    virtual void prepareResponce() = 0;

protected:
};

struct ResponseInfo {
    // will set NULL in case of request POST or PUT
    ContentTypeCollection*  m_contentTypes;
    std::string             m_hostPort;
    std::string             m_serverRoot;
    char*                   m_requestLocation = NULL;
    int                     m_clientFd = -1;
};
